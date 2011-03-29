/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/compiler.hh
/// \brief Specific features from some compilers.

#ifndef LIBPORT_COMPILER_HH
# define LIBPORT_COMPILER_HH

# include <libport/config.h>
# include <libport/preproc.hh>

/*----------------.
| __attribute__.  |
`----------------*/

# ifdef _MSC_VER

#  define __attribute__(a)
#  define ATTRIBUTE_ALWAYS_INLINE __forceinline
#  define ATTRIBUTE_DEPRECATED __declspec(deprecated)
#  if defined STATIC_BUILD
#    define ATTRIBUTE_DLLEXPORT
#    define ATTRIBUTE_DLLIMPORT
#  else
#    define ATTRIBUTE_DLLEXPORT __declspec(dllexport)
#    define ATTRIBUTE_DLLIMPORT __declspec(dllimport)
#  endif
#  define ATTRIBUTE_MALLOC  /* FILLME */
#  define ATTRIBUTE_NOINLINE __declspec(noinline)
#  define ATTRIBUTE_NORETURN __declspec(noreturn)
#  define ATTRIBUTE_NOTHROW  __declspec(nothrow)

/// Declare a function whose features printf-like arguments.
/// \param Format   the number of the printf-like format string.
///                 First argument is numbered 1, not 0.
///                 In a class, argument 1 is "this", so first visible
///                 argument is 2.
/// \param FirstArg The number of the first varargs.  Should point to
///                 the "..." in the argument list.
#  define ATTRIBUTE_PRINTF(Format, FirstArg)
#  define ATTRIBUTE_STDCALL  __stdcall
#  define ATTRIBUTE_UNUSED_RESULT /* FILLME */
#  define ATTRIBUTE_USED          /* FILLME */

# endif // _MSC_VER

# if !defined __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
#  if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 5) || __STRICT_ANSI__
#   define __attribute__(Spec) /* empty */
#  else
     // GCC 3.4 does not support "always_inline" without "inline".
#    define ATTRIBUTE_ALWAYS_INLINE inline __attribute__((__always_inline__))
#    define ATTRIBUTE_DEPRECATED __attribute__((__deprecated__))

// GCC 3 does not seem to support visibility, and generates tons of
// annoying error messages.
//
// See also http://gcc.gnu.org/wiki/Visibility.
#    if 4 <= __GNUC__
#      define ATTRIBUTE_DLLEXPORT __attribute__((visibility("default")))
#    else
#      define ATTRIBUTE_DLLEXPORT
#    endif
#    define ATTRIBUTE_DLLIMPORT ATTRIBUTE_DLLEXPORT

#    define ATTRIBUTE_MALLOC __attribute__((__malloc__))
#    define ATTRIBUTE_NOINLINE __attribute__((__noinline__))
#    define ATTRIBUTE_NORETURN __attribute__((__noreturn__))
#    define ATTRIBUTE_NOTHROW  __attribute__((__nothrow__))
#    define ATTRIBUTE_PRINTF(Format, FirstArg)                  \
       __attribute__((format(printf, Format, FirstArg)))

// Only on i386 architectures.  Left undefined on purpose for other
// architectures.
#    if defined __i386__
#     define ATTRIBUTE_STDCALL __attribute__((__stdcall__))
#    endif

#    define ATTRIBUTE_UNUSED_RESULT __attribute__((warn_unused_result))
#    define ATTRIBUTE_USED __attribute__((used))
#  endif
# endif // !defined __attribute__

// For throw() vs. nothrow, see
// http://www.nabble.com/Rest-of-trivial-decorations-td23114765.html
# ifdef __cplusplus
#  undef ATTRIBUTE_NOTHROW
#  define ATTRIBUTE_NOTHROW throw()
# endif


/*----------------------.
| __PRETTY_FUNCTION__.  |
`----------------------*/

// __PRETTY_FUNCTION__ is a GNU extension.  MSVC has something somewhat
// similar although it's less pretty.
# ifdef _MSC_VER
#  define __PRETTY_FUNCTION__ __FUNCTION__
# endif // _MSC_VER


/*--------------.
| LIBPORT_NOP.  |
`--------------*/

// A no-op for macros that should expand to nothing, but not result in
// warnings such as "warning: empty body in an if-statement".

# define LIBPORT_NOP                            \
  ((void) 0)

/*--------------.
| LIBPORT_USE.  |
`--------------*/

// Declare that variables/arguments are used.
//
// LIBPORT_USE(Var1, Var2)

# define LIBPORT_USE(...)                               \
  do {                                                  \
    BOOST_PP_SEQ_FOR_EACH(LIBPORT_USE_, ,               \
                          LIBPORT_LIST(__VA_ARGS__,))   \
  } while (false)

# define LIBPORT_USE_(_, __, Var)               \
  (void) Var;

/*------------------.
| LIBPORT_SECTION.  |
`------------------*/

// Each section must be pre-declared with its access rights, to do so, you
// should include compiler-section.hh
# ifdef _MSC_VER
#  define LIBPORT_SECTION(Name) __declspec(allocate(#Name))
# else
#  define LIBPORT_SECTION(Name) __attribute__((section(#Name)))
# endif

/*-------------------.
| LIBPORT_COLD/HOT.  |
`-------------------*/

# ifdef _MSC_VER
#  define LIBPORT_COLD
#  define LIBPORT_HOT
# else
#  define LIBPORT_COLD __attribute__ ((cold))
#  define LIBPORT_HOT  __attribute__ ((hot))
# endif

#endif // !LIBPORT_COMPILER_HH
