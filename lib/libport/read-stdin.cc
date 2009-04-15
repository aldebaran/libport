#include <libport/config.h>

#include <boost/lexical_cast.hpp>

#include <libport/cstring>
#include <libport/detect-win32.h>

#if defined LIBPORT_WIN32 || defined WIN32
# include <libport/windows.hh>
# include <libport/lockable.hh>
# include <iostream>
#else
# if ! defined LIBPORT_URBI_ENV_AIBO
#  include <sys/select.h>
# endif
# include <errno.h>
#endif

#include <libport/compiler.hh>
#include <libport/read-stdin.hh>
#include <libport/exception.hh>


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

#elif defined LIBPORT_URBI_ENV_AIBO

  /*-------.
  | AIBO.  |
  `-------*/

  std::string
  read_stdin()
  {
    return std::string();
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
    if (r <= -1)
      throw exception::Exception
        (std::string("select error on stdin: ") + strerror(errno));
    else if (r>0)
    {
      char buf[BUFSIZ];
      r = read(0, buf, sizeof buf);
      if (r <= 0) // EOF counts as an 'error'.
        throw exception::Exception
          (std::string("read error on stdin: ")
           + (r ? strerror(errno) : "EOF"));
      else
	return std::string(buf, r);
    }
    else
      return std::string();
  }

#endif

}
