#ifndef LIBPORT_COMPILATION_HH
# define LIBPORT_COMPILATION_HH

# include <libport/config.h>
# include <libport/compiler.hh>

# ifdef LIBPORT_COMPILATION_MODE_SPEED
#  define LIBPORT_SPEED_INLINE inline
   // GCC 3.4 does not support "always_inline" without "inline".
#  define LIBPORT_SPEED_ALWAYS_INLINE ATTRIBUTE_ALWAYS_INLINE
# else
#  define LIBPORT_SPEED_INLINE
#  define LIBPORT_SPEED_ALWAYS_INLINE
# endif

#endif
