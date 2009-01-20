#ifndef TINYXML_EXPORT_HH
# define TINYXML_EXPORT_HH

# include <libport/detect-win32.h>

# ifdef WIN32
#  ifdef BUILDING_TINYXML
#   define TINYXML_API __declspec(dllexport)
#  else
#   define TINYXML_API __declspec(dllimport)
#  endif
# else
#  define TINYXML_API __attribute__((visibility("default")))
# endif

#endif // TINYXML_EXPORT_HH
