#ifndef SCHED_EXPORT_HH
# define SCHED_EXPORT_HH

# include <libport/detect-win32.h>

# ifdef WIN32
#  ifdef BUILDING_SCHED
#   define SCHED_API __declspec(dllexport)
#  else
#   define SCHED_API __declspec(dllimport)
#  endif
# else
#  define SCHED_API __attribute__((visibility("default")))
# endif

#endif // SCHED_EXPORT_HH
