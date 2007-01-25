/// \file libport/compiler.hh
/// \brief Specific features from some compilers.

#ifndef LIBPORT_COMPILER_HH
#  define LIBPORT_COMPILER_HH

# include <cassert>


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

#if ENABLE_DEBUG_TRACES
# define ECHO(Msg)							\
  std::cerr << __FILE__ << ":" << __LINE__ << ": "			\
            << __PRETTY_FUNCTION__ << ": "				\
            << Msg << std::endl
#else
# define ECHO(Msg) (void) 0
#endif

#define PING()  ECHO("ping");


/*---------------------------.
| passert -- Pretty assert.  |
`---------------------------*/

/// Same as assert, but on failure, dump \a Subject of std::cerr.
///
/// If there are several subjects, one may use '<<':
///  passert (p << " " << q, p == q);

# ifdef NDEBUG
#  define passert(Subject, Assertion)       ((void)0)
# else

#  define passert(Subject, Assertion)		\
  ((void) ((Assertion)				\
           ? 0					\
           : __passert (Subject, # Assertion,	\
                       __FILE__, __LINE__)))

#  define __passert(Subject, e, file, line)				\
  (std::cerr								\
     << file << ':' << line << ": failed assertion: " << e << std::endl	\
     << file << ':' << line << ": with "				\
     << #Subject << " = " << Subject << std::endl,			\
   abort(),								\
   0)

# endif // !NDEBUG


# endif // !LIBPORT_COMPILER_HH
