#ifndef LIBPORT_EXPORT_HH
# define LIBPORT_EXPORT_HH

# include <libport/detect-win32.h>

# ifdef WIN32
#  ifdef BUILDING_LIBPORT
#   define LIBPORT_API __declspec(dllexport)
#  else
#   define LIBPORT_API __declspec(dllimport)
#  endif
# else
#  define LIBPORT_API __attribute__((visibility("default")))
# endif

#endif // LIBPORT_EXPORT_HH
