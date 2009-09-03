/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/assert.hh
/// \brief Provide nice assert like macros.

#ifndef LIBPORT_ASSERT_HH
# define LIBPORT_ASSERT_HH

# include <cassert>
# include <cstdlib>

// We have mysterious random aborts on OSX which is compiled with
// NDEBUG.  Temporarily disable NDEBUG to have verbose paborts.
# ifndef LIBPORT_ASSERT_VERBOSE
#  if defined NDEBUG
#   define LIBPORT_ASSERT_VERBOSE 0
#  else
#   define LIBPORT_ASSERT_VERBOSE 1
#  endif
# endif

# include <iostream> // std::cerr
# include <libport/cstring> // libport::strerror.

# include <libport/compiler.hh>

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


/*--------------.
| ASSERT_ECHO.  |
`--------------*/

# if LIBPORT_ASSERT_VERBOSE
#  define ASSERT_ECHO(File, Line, Message)                      \
  std::cerr << File << ":" << Line << ": " << Message << std::endl
# else
#  define ASSERT_ECHO(File, Line, Message)      \
  (void) 0
# endif



/*-------------------------.
| pabort -- Pretty abort.  |
`-------------------------*/

/// \def pabort(Msg)
///
/// Same as abort, but if LIBPORT_ASSERT_VERBOSE is set, report the
/// Msg using the operator<< on std::cerr.  Msg may include << itself.
/// So if Msg is complex, beware of predence issues with << and use
/// parens on the invocation side.

# define pabort(Msg)                                      \
  (ASSERT_ECHO(__FILE__, __LINE__, "abort: " << Msg),     \
   libport::abort(), 0)


/*-----------------------------------------------------.
| __passert -- Pretty assert with additional message.  |
`-----------------------------------------------------*/

/// \def __passert(Assertion, Message)
/// Same as assert, but on failure, dump \a Message on std::cerr.
# define __passert(Assertion, Message)			\
  ((void) ((Assertion)					\
	   ? 0						\
	   : pabort(Message)))

/*---------------------------.
| passert -- Pretty assert.  |
`---------------------------*/

/// \def passert(Subject, Assertion)
/// Same as assert, but on failure, dump \a Subject of std::cerr.
#  define passert(Subject, Assertion)                           \
  __passert(Assertion,                                          \
            "failed assertion: " << #Assertion << std::endl	\
            << "\t with " << #Subject << " = " << Subject)


/*----------------------------------------------.
| errabort -- perror (well, strerror) + abort.  |
`----------------------------------------------*/

/// \def errabort(Err, Msg)
# define errabort(Err, Msg)                     \
  pabort(libport::strerror(Err) << ": " << Msg)

/// \def errnoabort(Msg)
# define errnoabort(Msg)                        \
  errabort(errno, Msg)


/*--------------------------------------------------------.
| assert_exp -- Require a non-null value, and return it.  |
`--------------------------------------------------------*/

# if LIBPORT_ASSERT_VERBOSE
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
      ASSERT_ECHO(file, line, "failed assertion: " << msg);
      libport::abort();
    }
    return t;
  }
}

#  define assert_exp(Obj)		\
  libport::assert_exp_(Obj, __FILE__, __LINE__ , #Obj)
# else // !LIBPORT_ASSERT_VERBOSE
#  define assert_exp(Obj)		(Obj)
# endif // !LIBPORT_ASSERT_VERBOSE


/*---------------------------------------------------------------.
| assert_comp -- compare two values, show both of them if fail.  |
`---------------------------------------------------------------*/

#  define DEFINE_ASSERT_OP(OpName, Op)                                  \
namespace libport                                                       \
{                                                                       \
  template <typename T, typename U>                                     \
  inline                                                                \
  void                                                                  \
  assert_ ## OpName(const T& lhs, const U& rhs,                         \
                    const char* lstr, const char* rstr,                 \
                    const char* file, int line)                         \
  {                                                                     \
    if (!(lhs Op rhs))                                                  \
    {                                                                   \
      ASSERT_ECHO(file, line,                                           \
                  "failed assertion: " << lstr << " " #Op " " << rstr); \
      ASSERT_ECHO(file, line, "  with " << lstr << " = " << lhs);       \
      ASSERT_ECHO(file, line, "  with " << rstr << " = " << rhs);       \
      libport::abort();                                                 \
    }                                                                   \
  }                                                                     \
}

  DEFINE_ASSERT_OP(eq, ==)
  DEFINE_ASSERT_OP(ge, >=)
  DEFINE_ASSERT_OP(gt, > )
  DEFINE_ASSERT_OP(le, <=)
  DEFINE_ASSERT_OP(lt, < )
  DEFINE_ASSERT_OP(ne, !=)

# if LIBPORT_ASSERT_VERBOSE
#  define assert_eq(A, B) ::libport::assert_eq(A, B, #A, #B, __FILE__, __LINE__)
#  define assert_ge(A, B) ::libport::assert_ge(A, B, #A, #B, __FILE__, __LINE__)
#  define assert_gt(A, B) ::libport::assert_gt(A, B, #A, #B, __FILE__, __LINE__)
#  define assert_le(A, B) ::libport::assert_le(A, B, #A, #B, __FILE__, __LINE__)
#  define assert_lt(A, B) ::libport::assert_lt(A, B, #A, #B, __FILE__, __LINE__)
#  define assert_ne(A, B) ::libport::assert_ne(A, B, #A, #B, __FILE__, __LINE__)

# else // !LIBPORT_ASSERT_VERBOSE

#  define assert_eq(A, B) assert(A == B)
#  define assert_ge(A, B) assert(A >= B)
#  define assert_gt(A, B) assert(A >  B)
#  define assert_le(A, B) assert(A <= B)
#  define assert_lt(A, B) assert(A <  B)
#  define assert_ne(A, B) assert(A != B)
# endif

#endif // !LIBPORT_ASSERT_HH
