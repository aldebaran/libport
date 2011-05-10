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

# ifdef LIBPORT_HAVE_VALGRIND_VALGRIND_H
#  ifndef USE_VALGRIND
#   define NVALGRIND 1
#  endif
# else
#  define RUNNING_ON_VALGRIND 0
#  define NVALGRIND 1
#  ifdef USE_VALGRIND
#   undef USE_VALGRIND
#  endif
# endif

# ifndef NVALGRIND
#  include <valgrind/valgrind.h>
#  include <valgrind/memcheck.h>
# endif

/*-------------.
| INSTRUMENT.  |
`-------------*/

# define INSTRUMENTFLAGS(Flags)					\
  const char* LIBPORT_CAT(libport_instrument, __LINE__) =	\
    "INSTRUMENTFLAGS=" __HERE__ ":" #Flags

# define INSTRUMENT_DISABLE()                   \
  INSTRUMENTFLAGS(--mode=none)

/*--------------------------------.
| Valgrind Memory Access Rights.  |
`--------------------------------*/

// These macros are used to ensure that no other part of the program can
// access the variables.  This are means to check for bad memory writes
// which are made on specific variables.
# ifndef NVALGRIND

#  define MEM(Right, Addr, Size)                        \
  LIBPORT_CAT(VALGRIND_MAKE_MEM_, Right) (Addr, Size)

#  define POOL_CREATE(pool, redZone, init)      \
  VALGRIND_CREATE_MEMPOOL(pool, redZone, init);

#  define POOL_ALLOC(pool, addr, size)          \
  VALGRIND_MEMPOOL_ALLOC(pool, addr, size);

#  define POOL_FREE(pool, addr)                 \
  VALGRIND_MEMPOOL_FREE(pool, addr);

# else
#  define MEM(Right, Addr, Size)
#  define POOL_CREATE(pool, redZone, init)
#  define POOL_ALLOC(pool, addr, size)
#  define POOL_FREE(pool, addr)
# endif

# define MEM_NOACCESS(Addr, Size)  MEM(NOACCESS, Addr, Size)
# define MEM_UNDEFINED(Addr, Size) MEM(UNDEFINED, Addr, Size)
# define MEM_DEFINED(Addr, Size)   MEM(DEFINED, Addr, Size)

# define VAR_NOACCESS(Val)  MEM_NOACCESS(&Val, sizeof(Val))
# define VAR_UNDEFINED(Val) MEM_UNDEFINED(&Val, sizeof(Val))
# define VAR_DEFINED(Val)   MEM_DEFINED(&Val, sizeof(Val))


#endif
