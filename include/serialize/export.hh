#ifndef SERIALIZE_EXPORT_HH
# define SERIALIZE_EXPORT_HH

# include <libport/detect-win32.h>

# ifdef WIN32
#  ifdef BUILDING_SERIALIZE
#   define SERIALIZE_API __declspec(dllexport)
#  else
#   define SERIALIZE_API __declspec(dllimport)
#  endif
# else
#  define SERIALIZE_API __attribute__((visibility("default")))
# endif

#endif // SERIALIZE_EXPORT_HH
