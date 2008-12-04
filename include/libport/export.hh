#ifndef LIBPORT_EXPORT_HH
# define LIBPORT_EXPORT_HH

# include <libport/detect-win32.h>

# ifdef BUILDING_LIBPORT
#  ifdef WIN32
#   define LIBPORT_API __declspec(dllexport)
#  else
#    define LIBPORT_API __attribute__((visibility("default")))
#  endif
# endif

# ifndef LIBPORT_API
#  ifdef WIN32
#   define LIBPORT_API __declspec(dllimport)
#  else
#   define LIBPORT_API
#  endif
# endif

#endif
