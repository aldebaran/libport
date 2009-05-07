/// \file libport/assert.hh
/// \brief Provide nice assert like macros.

#ifndef LIBPORT_ASSERT_HH
# define LIBPORT_ASSERT_HH

# include <cassert>
# include <cstdlib>

// We have mysterious random aborts on OSX which is compiled with
// NDEBUG.  Temporarily disable NDEBUG to have verbose paborts.
# if defined NDEBUG
#  define LIBPORT_ASSERT_VERBOSE 0
# else
#  define LIBPORT_ASSERT_VERBOSE 1
# endif

# include <iostream> // std::cerr
# include <libport/cstdio> // libport::strerror.

# include <libport/compiler.hh>
# include <libport/format.hh>

# ifdef _MSC_VER
#  include <crtdbg.h>
# endif

namespace libport
{
  /// A wrapper to std::abort to ensure that it is declared as
  /// noreturn (which is not the case of std::abort with MSVC).
  LIBPORT_API
  ATTRIBUTE_NORETURN
  void abort();
}

/*---------------------------.
| passert -- Pretty assert.  |
`---------------------------*/

/// \def passert(Subject, Assertion)
/// Same as assert, but on failure, dump \a Subject of std::cerr.
# if ! LIBPORT_ASSERT_VERBOSE
#  define passert(Subject, Assertion)
# else

#  define passert(Subject, Assertion)			\
  ((void) ((Assertion)					\
	   ? 0						\
	   : __passert(Subject, Assertion)))

#  define __passert(Subject, Assertion)                         \
  pabort("failed assertion: " << #Assertion << std::endl	\
         << "\t with "                                          \
         << #Subject << " = " << Subject)

# endif // LIBPORT_ASSERT_VERBOSE


/*-------------------------.
| pabort -- Pretty abort.  |
`-------------------------*/

/// \def pabort(Msg)
///
/// Same as abort, but when NDEBUG is not set, report the Msg using
/// the operator<< on std::cerr.  Msg may include << itself.  So
/// if Msg is complex, beware of predence issues with << and use parens
/// on the invocation side.

# define pabort(Msg)                                    \
  (std::cerr << __FILE__ ":" << __LINE__                \
             << ": abort: " << Msg << std::endl,        \
   libport::abort(), 0)

/*----------------------------------------------.
| errabort -- perror (well, strerror) + abort.  |
`----------------------------------------------*/

/// \def errabort(Msg)
# define errabort(Msg)    \
  pabort(libport::strerror(errno) << ": " << Msg)



/*--------------------------------------------------------.
| assert_exp -- Require a non-null value, and return it.  |
`--------------------------------------------------------*/

# if ! LIBPORT_ASSERT_VERBOSE
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
      libport::abort();
    }
    return t;
  }
}

#  define assert_exp(Obj)		\
  libport::assert_exp_(Obj, __FILE__, __LINE__ , #Obj)
# endif // LIBPORT_ASSERT_VERBOSE


/*-------------------------------------------------------------.
| assert_comp -- compare two values, show both of them if fail |
`-------------------------------------------------------------*/

# if ! LIBPORT_ASSERT_VERBOSE
#  define assert_eq
#  define assert_ne
# else
#  define assert_op(Op, A, B)                                           \
  if (!(A Op B))                                                        \
    pabort(::libport::format(#A " " #Op " " #B " (%s " #Op " %s)", A, B)) \

#  define assert_eq(A, B) assert_op(==, A, B)
#  define assert_ne(A, B) assert_op(!=, A, B)
#  define assert_lt(A, B) assert_op(< , A, B)
#  define assert_le(A, B) assert_op(<=, A, B)
#  define assert_gt(A, B) assert_op(> , A, B)
#  define assert_ge(A, B) assert_op(>=, A, B)
# endif

#endif // !LIBPORT_ASSERT_HH
