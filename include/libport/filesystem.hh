#ifndef LIBPORT_FILESYSTEM_HH
# define LIBPORT_FILESYSTEM_HH

# include <libport/detect-win32.h>

# ifdef WIN32
#  include <direct.h>
namespace std
{
  int chdir(char* path)
  {
    return _chdir(path);
  }
}
# endif

#endif
