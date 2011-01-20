/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
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
#include <libport/debug.hh>
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

#define FAIL_(Format, ...)                                              \
  do {                                                                  \
    GD_FERROR(Format, ## __VA_ARGS__);                                  \
    throw libport::Exception(libport::format(Format, ## __VA_ARGS__));  \
  } while (false)

#define FAIL(Format, ...)                                               \
  FAIL_(Format ": %s", ## __VA_ARGS__, strerror(errno))


#define INFO_(Format, ...)                                              \
  do {                                                                  \
    GD_FINFO_DEBUG(Format, ## __VA_ARGS__);                             \
    throw libport::Exception(libport::format(Format, ## __VA_ARGS__));  \
  } while (false)

#define INFO(Format, ...)                                               \
  INFO_(Format ": %s", ## __VA_ARGS__, strerror(errno))


GD_CATEGORY(Libport.ReadFile);


namespace libport
{

#if defined LIBPORT_WIN32 || defined WIN32

  /*----------.
  | Windows.  |
  `----------*/

  struct ConsumerData
  {
    ConsumerData()
      : fd(-1)
      , buffer()
      , lock()
      , exception(0)
    {}

    bool started()
    {
      return fd != -1;
    }

    void clear()
    {
      delete exception;
      exception = 0;
      fd = -1;
    }
    int fd;
    std::string buffer;
    Lockable lock;
    // If set, an exception to throw.
    libport::Exception *exception;
  };

  static DWORD WINAPI
  readThread(void* d)
  {
    ConsumerData& data = *reinterpret_cast<ConsumerData*>(d);
    try
    {
      while (true)
      {
        static char buf[LIBPORT_BUFSIZ];
        int r = read(data.fd, buf, sizeof buf);
        if (r < 0)
          FAIL("read error on fd = %s", data.fd);
        else if (r == 0) // EOF counts as an 'error' but less verbose.
        {
          INFO("read error on fd = %s: EOF", data.fd);
          break;
        }
        BlockLock bl(data.lock);
        data.buffer.append(buf, r);
      }
    }
    catch (const libport::Exception& e)
    {
      // Don't let exceptions get out of a thread.
      data.exception = new libport::Exception(e);
    }
    return 0;
  }

  std::string
  read_fd(int fd)
  {
    static ConsumerData data;

    BlockLock bl(data.lock);
    if (!data.started())
    {
      data.fd = fd;
      unsigned long id;
      CreateThread(NULL, 0, &readThread, &data, 0, &id);
    }

    if (data.exception && data.buffer.empty())
    {
      libport::Exception exception = *data.exception;
      data.clear();
      throw exception;
    }
    std::string res;
    swap(data.buffer, res);
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
        if (errno != EINTR)
          FAIL("select error on fd=%d", fd);
        else
          return 0;
      else if (r == 0)
        return 0;
    }

    // Read.
    {
      int r = read(fd, buf, len);
      if (r < 0)
        if (errno != EINTR)
          FAIL("read error on fd = %s", fd);
        else
          return 0;
      else if (r == 0) // EOF counts as an 'error' but less verbose.
      {
        if (fd == 0)
          INFO_("stdin (%s) closed (EOF), interactive mode disabled.", fd);
        else
          INFO_("End Of File on file descriptor %s", fd);
      }
      return r;
    }
  }

  std::string
  read_fd(int fd)
  {
    char buf[LIBPORT_BUFSIZ];
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
