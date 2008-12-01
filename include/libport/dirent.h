#ifndef LIBPORT_DIRENT_H
# define LIBPORT_DIRENT_H

# include <libport/detect-win32.h>

# ifdef WIN32

#  include <libport/windows.hh>
#  include <io.h>
#  include <string>
#  include <windows.h>

#  define NAME_MAX 255

extern "C"
{
  struct dirent
  {
    char d_name[NAME_MAX + 1];
  };

  struct DIR;

  /* This does not implement the full {open,close,read}dir API. The
     only purpose is to enable to transparently list directories under
     Windows. Because of the underlying primitives, the behaviour also
     differs on some point: for instance, opendir is systematically
     successful, even if the directory does not exist. If such a
     difference becomes problematic in some future situation, the code
     must be adapted.  */

  LIBPORT_API DIR* opendir(const char* name);
  LIBPORT_API int closedir(DIR* dir);
  LIBPORT_API dirent* readdir(DIR *dir);
}

# else
#  include <sys/types.h>
#  include <dirent.h>
# endif

#endif
