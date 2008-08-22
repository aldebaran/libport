#ifndef LIBPORT_EXPORT_HH
# define LIBPORT_EXPORT_HH
#include <libport/detect-win32.h>

# if defined WIN32
#  if defined BUILDING_LIBPORT
#   define LIBPORT_API __declspec(dllexport)
#  else
#   define LIBPORT_API __declspec(dllimport)
#  endif
# else
# define LIBPORT_API
# endif


#endif
