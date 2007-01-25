/// \file libport/assert.hh
/// \brief Provide nice assert like macros.

#ifndef LIBPORT_ASSERT_HH
#  define LIBPORT_ASSERT_HH

# include <cassert>

# if ! defined NDEBUG
#  include <iostream> // std::cerr
# endif

/*---------------------------.
| passert -- Pretty assert.  |
`---------------------------*/

/// \def passert(Subject, Assertion)
/// Same as assert, but on failure, dump \a Subject of std::cerr.
///
/// If there are several subjects, one may use '<<':
///  passert (p << " " << q, p == q);

# ifdef NDEBUG
#  define passert(Subject, Assertion)       ((void)0)
# else

#  define passert(Subject, Assertion)			\
  ((void) ((Assertion)					\
	   ? 0						\
	   : __passert (__FILE__ ":" <<  __LINE__,	\
			Subject, Assertion)))

#  define __passert(Loc, Subject, Assertion)				\
  (std::cerr								\
     << Loc << ": failed assertion: " << #Assertion << std::endl	\
     << Loc << ": with "						\
     << #Subject << " = " << Subject << std::endl,			\
   abort(),								\
   0)

# endif // !NDEBUG



/*-------------------------.
| pabort -- Pretty abort.  |
`-------------------------*/

/// \def pabort(Msg)
///
/// Same as abort, but when NDEBUG is not set, report the Msg using
/// the operator<< on std::cerr.  Msg may include << itself.  So
/// if Msg is complex, beware of predence issues with << and use parens
/// on the invocation side.

# ifdef NDEBUG
#  define pabort(Msg)       abort()
# else

#  define pabort(Msg)					\
  ((void) (__pabort (__FILE__ ":" << __LINE__, Msg)))

#  define __pabort(Loc, Msg)						\
  (std::cerr << Loc << ": abort: " << Msg << std::endl,			\
   abort())

# endif // !NDEBUG


# endif // !LIBPORT_COMPILER_HH
