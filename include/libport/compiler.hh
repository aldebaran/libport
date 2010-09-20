/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
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

#    define ATTRIBUTE_NOINLINE __attribute__((__noinline__))
#    define ATTRIBUTE_NORETURN __attribute__((__noreturn__))
#    define ATTRIBUTE_NOTHROW  __attribute__((__nothrow__))
#    define ATTRIBUTE_PRINTF(Format, FirstArg) \
         __attribute__((format(printf, Format, FirstArg)))
#    define ATTRIBUTE_STDCALL __attribute__((__stdcall__))
#    define ATTRIBUTE_UNUSED_RESULT __attribute__((warn_unused_result))
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

#endif // !LIBPORT_COMPILER_HH
