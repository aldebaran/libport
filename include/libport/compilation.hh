/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

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
