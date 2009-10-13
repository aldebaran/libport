/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <fstream>
#include <iterator>
#include <libport/cassert>
#include <libport/cerrno>
#include <libport/compiler.hh>
#include <libport/config.h>
#include <libport/cstdio>
#include <libport/cstring>
#include <libport/detect-win32.h>
#include <libport/exception.hh>
#include <libport/format.hh>
#include <libport/lexical-cast.hh>
#include <libport/read-stdin.hh>
#include <libport/sys/select.h>
#include <libport/sys/stat.h>

#if defined LIBPORT_WIN32 || defined WIN32
# include <libport/windows.hh>
# include <libport/lockable.hh>
# include <iostream>
#endif

#define FAIL_(Format, ...)                                      \
  throw libport::Exception(libport::format(Format, ## __VA_ARGS__))

#define FAIL(Format, ...)                                       \
  FAIL_(Format ": %s", ## __VA_ARGS__, strerror(errno))

namespace libport
{

#if defined LIBPORT_WIN32 || defined WIN32

  /*----------.
  | Windows.  |
  `----------*/

  struct ConsumerData
  {
    int fd;
    std::string read;
    Lockable lock;
  };

  static DWORD WINAPI
  readThread(void* d)
  {
    data_type&data = *reinterpret_cast<ConsumerData*>(d);
    while (true)
    {
      static char buf[BUFSIZ];
      int r = read(data.fd, buf, sizeof buf);
      if (r < 0)
        FAIL("read error on fd = %s", data.fd);
      else if (r == 0) // EOF counts as an 'error'.
        FAIL_("read error on fd = %s: EOF", data.fd);
      BlockLock bl(data.lock);
      data.read.append(buf, r);
    }
  }

  std::string
  read_fd(int fd)
  {
    static bool started = false;
    static data_type data;
    if (!started)
    {
      data.handle = handle;
      started = true;
      unsigned long id;
      CreateThread(NULL, 0, &readThread, &data, 0, &id);
      return std::string();
    }
    BlockLock bl(data.lock);
    std::string res;
    swap(data.first, res);
    return res;
  }

#else

  /*-------.
  | Unix.  |
  `-------*/

  size_t
  read_fd(int fd, char* buf, size_t len)
  {
    // Select.
    {
      fd_set fds;
      FD_ZERO(&fds);
      FD_SET(fd, &fds);

      struct timeval tv;
      tv.tv_sec = tv.tv_usec = 0;

      int r = select(fd + 1, &fds, 0, 0, &tv);
      if (r < 0)
        FAIL("select error on fd=%d", fd);
      else if (r == 0)
        return 0;
    }

    // Read.
    {
      int r = read(0, buf, len);
      if (r < 0)
        FAIL("read error on fd = %s", fd);
      else if (r == 0) // EOF counts as an 'error'.
        FAIL_("read error on fd = %s: EOF", fd);
      else
	return r;
    }
  }

  std::string
  read_fd(int fd)
  {
    char buf[BUFSIZ];
    size_t len = read_fd(fd, buf, sizeof buf);
    return std::string(buf, len);
  }

#endif

  std::string
  read_stdin()
  {
    return read_fd(STDIN_FILENO);
  }

  std::string
  read_file(const std::string& file)
  {
    if (file == "/dev/stdin")
      return read_stdin();
    else
    {
      // Or should we simply use tellg to get the size?
      struct stat st;
      if (stat(file.c_str(), &st) < 0)
        FAIL("cannot stat `%s'", file);

      std::string res;
      res.reserve(st.st_size);
      std::ifstream is(file.c_str(), std::ios::binary);
      if (is.fail())
        FAIL("cannot open `%s' for reading", file);
      std::copy(std::istreambuf_iterator<char>(is.rdbuf()),
                std::istreambuf_iterator<char>(),
                std::back_inserter(res));

      if (is.fail())
        FAIL("cannot read `%s'", file);
      // FIXME: We need to check for more error kinds.
      return res;
    }
  }

}
