/// \file libport/assert.hh
/// \brief Provide nice assert like macros.

#ifndef LIBPORT_ASSERT_HH
# define LIBPORT_ASSERT_HH

# include <cassert>
# include <cstdlib>

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
#  define passert(Subject, Assertion)       ((void)Subject)
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
   std::abort(),                                                        \
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
   std::abort())

# endif // !NDEBUG


/*--------------------------------------------------------.
| assert_exp -- Require a non-null value, and return it.  |
`--------------------------------------------------------*/

# ifdef NDEBUG
#  define assert_exp(Obj)		(Obj)
# else
// Basically, an assert that can be used in an expression.  I meant to
// use "nonnull", but this name is unused by libstdc++, so the #define
// breaks everything.
namespace libport
{
  template <typename T>
  inline
  T
  assert_exp_(T t, const char* file, int line, const char* msg)
  {
    if (!t)
    {
      std::cerr
	<< file << ": " << line << ": failed assertion: " << msg << std::endl;
      std::abort();
    }
    return t;
  }
}

#  define assert_exp(Obj)		\
  libport::assert_exp_(Obj, __FILE__, __LINE__ , #Obj)
# endif // !NDEBUG


#endif // !LIBPORT_ASSERT_HH
