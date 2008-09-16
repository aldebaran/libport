/// \file libport/unistd.cc

#include <libport/unistd.h>
#include <libport/config.h>

# ifdef WIN32
extern "C"
{
  int chdir(const char* path);
}
#endif

#if defined LIBPORT_URBI_ENV_AIBO

char*
getcwd(char* buf, size_t size)
{
  if (buf == 0 || size <= 3)
    return 0;
  strncpy (buf, "/MS", size);
  return buf;
}

#endif // LIBPORT_URBI_ENV_AIBO
