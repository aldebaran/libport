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

  typedef std::pair<std::string, Lockable> data_type;
  static DWORD WINAPI
  readThread(void* d)
  {
    data_type&data = *reinterpret_cast<data_type*>(d);
    while (true)
    {
      static char buf[BUFSIZ];
      DWORD count = 0;
      ReadFile(GetStdHandle(STD_INPUT_HANDLE), buf, sizeof buf, &count, 0);
      BlockLock bl(data.second);
      data.first.append(buf, count);
    }
  }

  std::string
  read_stdin()
  {
    static bool started = false;
    static data_type data;
    if (!started)
    {
      started = true;
      unsigned long id;
      CreateThread(NULL, 0, &readThread, &data, 0, &id);
      return std::string();
    }
    BlockLock bl(data.second);
    std::string res = data.first;
    data.first.clear();
    return res;
  }

#else

  /*-------.
  | Unix.  |
  `-------*/

  std::string
  read_stdin()
  {
    fd_set fd;
    FD_ZERO(&fd);
    FD_SET(0,&fd);
    struct timeval tv;
    tv.tv_sec = tv.tv_usec = 0;
    int r = select(1, &fd, 0, 0, &tv);
    if (r < 0)
      FAIL("select error on stdin");
    else if (r == 0)
      return std::string();
    else
    {
      char buf[BUFSIZ];
      r = read(0, buf, sizeof buf);
      if (r < 0)
        FAIL("read error on stdin");
      else if (r == 0) // EOF counts as an 'error'.
        FAIL_("read error on stdin: EOF");
      else
	return std::string(buf, r);
    }
  }

#endif

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
