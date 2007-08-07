/// \file libport/compiler.hh
/// \brief Specific features from some compilers.

#ifndef LIBPORT_COMPILER_HH
# define LIBPORT_COMPILER_HH

# include "libport/config.h"

/*----------------.
| __attribute__.  |
`----------------*/

# ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
#  if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 5) || __STRICT_ANSI__
#   define __attribute__(Spec) /* empty */
#  endif
# endif


/*--------.
| SLEEP.  |
`--------*/

# ifdef LIBPORT_URBI_ENV_AIBO

// On aibo, make sure the log message arrives.

#  define SLEEP(C)					\
  do {							\
    long sum = 0;					\
    for (long i = 0; i < C * 100000000; ++i)		\
      sum += i;						\
  } while(0)

# else

#  define SLEEP(C) ((void) 0)

# endif

/*--------------.
| ECHO & PING.  |
`--------------*/

// Disable debug traces when compiling with NDEBUG.
# ifdef NDEBUG
#  undef ENABLE_DEBUG_TRACES
# endif

# ifdef ENABLE_DEBUG_TRACES

#  include <iostream>

#  define ECHO(Msg)					\
  do {							\
    std::cerr << __FILE__ ":" << __LINE__ << ": "	\
	      << __PRETTY_FUNCTION__ << ": "		\
	      << Msg << std::endl;			\
    SLEEP(1);						\
  } while (0)

# else
#  define ECHO(Msg) (void) 0
# endif

# define ECHO_CMD(Cmd)						\
  do {								\
    ECHO("Running " << #Cmd << "..." << std::endl);		\
    Cmd;							\
    ECHO("Running " << #Cmd << "... done" << std::endl);	\
  } while (0)

# define PING()  ECHO("ping")

#endif // !LIBPORT_COMPILER_HH
