#ifndef LIBPORT_COMPILATION_HH
# define LIBPORT_COMPILATION_HH

# include <libport/config.h>

# ifdef LIBPORT_OPTIMIZE_SPEED
#  define LIBPORT_SPEED_INLINE inline
#  define LIBPORT_SPEED_ALWAYS_INLINE __attribute__((always_inline))
# else
#  define LIBPORT_SPEED_INLINE
#  define LIBPORT_SPEED_ALWAYS_INLINE
# endif

#endif
