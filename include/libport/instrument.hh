/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_INSTRUMENT_HH
# define LIBPORT_INSTRUMENT_HH

# include <libport/config.h>
# include <libport/preproc.hh>

# ifdef LIBPORT_HAVE_VALGRIND_VALGRIND_H
#  include <valgrind/valgrind.h>
# else
#  define RUNNING_ON_VALGRIND 0
# endif

# define INSTRUMENTFLAGS(Flags)					\
  const char* LIBPORT_CAT(libport_instrument, __LINE__) =	\
    "INSTRUMENTFLAGS=" __HERE__ ":" #Flags

# define INSTRUMENT_DISABLE()                   \
  INSTRUMENTFLAGS(--mode=none)

#endif
