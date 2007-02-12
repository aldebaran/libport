/// \file libport/compiler.hh
/// \brief Specific features from some compilers.

#ifndef LIBPORT_COMPILER_HH
#  define LIBPORT_COMPILER_HH

/*----------------.
| __attribute__.  |
`----------------*/

# ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
#  if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 5) || __STRICT_ANSI__
#   define __attribute__(Spec) /* empty */
#  endif
# endif


/*--------------.
| ECHO & PING.  |
`--------------*/

# ifdef ENABLE_DEBUG_TRACES
#  define ECHO(Msg)							\
  std::cerr << __FILE__ << ":" << __LINE__ << ": "			\
	    << __PRETTY_FUNCTION__ << ": "				\
	    << Msg << std::endl
# else
#  define ECHO(Msg) (void) 0
# endif

# define PING()  ECHO("ping")

# endif // !LIBPORT_COMPILER_HH
