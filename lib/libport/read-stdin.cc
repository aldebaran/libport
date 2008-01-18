#include "libport/config.h"

#include "libport/read-stdin.hh"

namespace libport
{
std::string
read_stdin()
{
  char buf[1024];
# ifndef WIN32
  //select
  fd_set fd;
  FD_ZERO(&fd);
  FD_SET(0,&fd);
  struct timeval tv;
  tv.tv_sec = tv.tv_usec = 0;
  int r = select(1,&fd,0,0,&tv);
  if (r == -1)
  {
    perror ("select failed");
    //exit (EX_OSERR);
  }
  else if (r>0)
  {
    r = read(0, buf, sizeof buf -1); //-1 for the 0 we will insert
    if (r == -1)
    {
      perror ("read failed");
      //exit (EiX_OSERR);
    }
    if (r > 0)
      return std::string(buf, r);
  }
#else
  HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
  DWORD r = WaitForSingleObject(hstdin, 0);
  if (r != WAIT_TIMEOUT)
  {
    DWORD bytesRead;
    if (ReadFile(hstdin, buf, sizeof buf -1, &bytesRead, 0))
      return std::string(buf, bytesRead);
  }
#endif
  return std::string();
}
}
