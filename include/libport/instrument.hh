/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
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

// Keep NVALGRIND and USE_VALGRIND consistent.  Prefer NVALGRIND.
// NVALGRIND overrides USE_VALGRIND.
# if defined NVALGRIND
#  undef USE_VALGRIND
# endif

// USE_VALGRIND disables Valgrind.
# if ! defined USE_VALGRIND
#  define NVALGRIND 1
# endif

# ifdef LIBPORT_HAVE_VALGRIND_VALGRIND_H
// Load these even if NVALGRIND, as it takes care itself of disabling
// itself.
#  include <valgrind/valgrind.h>
#  include <valgrind/memcheck.h>
# else
#  define RUNNING_ON_VALGRIND 0
# endif

/*-------------.
| INSTRUMENT.  |
`-------------*/

// Always define it, even if NVALGRIND, as it protects us from running
// Valgrind in place where it does not work properly.  See for
// instance tests/libport/cstdlib.
# define INSTRUMENTFLAGS(Flags)					\
  const char* LIBPORT_CAT(libport_instrument, __LINE__) =		\
    "INSTRUMENTFLAGS=" __HERE__ ":" #Flags
# define INSTRUMENT_DISABLE()                   \
  INSTRUMENTFLAGS(--mode=none)


/*--------------------------------.
| Valgrind Memory Access Rights.  |
`--------------------------------*/

// These macros are used to ensure that no other part of the program can
// access the variables.  This are means to check for bad memory writes
// which are made on specific variables.
# if defined NVALGRIND
#  define MEM(Right, Addr, Size)           LIBPORT_NOP
#  define POOL_CREATE(Pool, RedZone, Init) LIBPORT_NOP
#  define POOL_ALLOC(Pool, Addr, Size)     LIBPORT_NOP
#  define POOL_FREE(Pool, Addr)		   LIBPORT_NOP
# else
#  define MEM(Right, Addr, Size)                        \
  LIBPORT_CAT(VALGRIND_MAKE_MEM_, Right) (Addr, Size)
# define POOL_CREATE(Pool, RedZone, Init)	\
  VALGRIND_CREATE_MEMPOOL(Pool, RedZone, Init)
# define POOL_ALLOC(Pool, Addr, Size)		\
  VALGRIND_MEMPOOL_ALLOC(Pool, Addr, Size)
# define POOL_FREE(Pool, Addr)			\
  VALGRIND_MEMPOOL_FREE(Pool, Addr)
# endif

# define MEM_NOACCESS(Addr, Size)  MEM(NOACCESS, Addr, Size)
# define MEM_UNDEFINED(Addr, Size) MEM(UNDEFINED, Addr, Size)
# define MEM_DEFINED(Addr, Size)   MEM(DEFINED, Addr, Size)

# define VAR_NOACCESS(Val)  MEM_NOACCESS(&Val, sizeof(Val))
# define VAR_UNDEFINED(Val) MEM_UNDEFINED(&Val, sizeof(Val))
# define VAR_DEFINED(Val)   MEM_DEFINED(&Val, sizeof(Val))

#endif
