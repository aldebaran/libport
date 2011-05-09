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

# define GCC_VERSION_GE(Major, Minor)                           \
  (defined __GNUC__                                             \
   && (Major < __GNUC__                                         \
       || Major == __GNUC__ && Minor <= __GNUC_MINOR__))

/*----------------.
| __attribute__.  |
`----------------*/

/* This feature is available in gcc versions 2.5 and later.  */
# if !defined __attribute__ && (defined _MSC_VER || ! GCC_VERSION_GE(2, 5))
#  define __attribute__(Spec) /* empty */
# endif


/*--------------------------------.
| ATTRIBUTE_DLLEXPORT/DLLIMPORT.  |
`--------------------------------*/

// GCC 3 does not seem to support visibility, and generates tons of
// annoying error messages.
# if GCC_VERSION_GE(4, 0)
#   define ATTRIBUTE_DLLEXPORT __attribute__((visibility("default")))
#   define ATTRIBUTE_DLLIMPORT ATTRIBUTE_DLLEXPORT

# elif defined _MSC_VER && ! defined STATIC_BUILD
#  define ATTRIBUTE_DLLEXPORT __declspec(dllexport)
#  define ATTRIBUTE_DLLIMPORT __declspec(dllimport)

# else
#  define ATTRIBUTE_DLLEXPORT
#  define ATTRIBUTE_DLLIMPORT
# endif

/*--------------.
| ATTRIBUTE_*.  |
`--------------*/

# if __GNUC__
     // GCC 3.4 does not support "always_inline" without "inline".
#  define ATTRIBUTE_ALWAYS_INLINE inline __attribute__((always_inline))
#  define ATTRIBUTE_CONST      __attribute__((const))
#  ifndef ATTRIBUTE_DEPRECATED
#   define ATTRIBUTE_DEPRECATED __attribute__((deprecated))
#  endif
#  define ATTRIBUTE_MALLOC     __attribute__((malloc))
#  define ATTRIBUTE_NOINLINE   __attribute__((noinline))
#  define ATTRIBUTE_NORETURN   __attribute__((noreturn))
#  define ATTRIBUTE_NOTHROW    __attribute__((nothrow))

/// Declare a function whose features printf-like arguments.
/// \param Format   the number of the printf-like format string.
///                 First argument is numbered 1, not 0.
///                 In a class, argument 1 is "this", so first visible
///                 argument is 2.
/// \param FirstArg The number of the first varargs.  Should point to
///                 the "..." in the argument list.
#  define ATTRIBUTE_PRINTF(Format, FirstArg)                  \
     __attribute__((format(printf, Format, FirstArg)))

#  define ATTRIBUTE_PURE __attribute__((pure))

// Only on i386 architectures.  Left undefined on purpose for other
// architectures.
#  if defined __i386__
#    define ATTRIBUTE_STDCALL    __attribute__((stdcall))
#  endif

#  define ATTRIBUTE_UNUSED_RESULT __attribute__((warn_unused_result))
#  define ATTRIBUTE_USED          __attribute__((used))

# elif defined _MSC_VER

#  define ATTRIBUTE_ALWAYS_INLINE __forceinline
#  define ATTRIBUTE_CONST
#  ifndef ATTRIBUTE_DEPRECATED
#    define ATTRIBUTE_DEPRECATED __declspec(deprecated)
#  endif
#  define ATTRIBUTE_MALLOC
#  define ATTRIBUTE_NOINLINE   __declspec(noinline)
#  define ATTRIBUTE_NORETURN   __declspec(noreturn)
#  define ATTRIBUTE_NOTHROW    __declspec(nothrow)
#  define ATTRIBUTE_PRINTF(Format, FirstArg)
#  define ATTRIBUTE_PURE
#  define ATTRIBUTE_STDCALL    __stdcall
#  define ATTRIBUTE_UNUSED_RESULT
#  define ATTRIBUTE_USED

# else // !__GNUC__ && !_MSC_VER, e.g., Swig.

#  define ATTRIBUTE_ALWAYS_INLINE
#  define ATTRIBUTE_CONST
#  ifndef ATTRIBUTE_DEPRECATED
#    define ATTRIBUTE_DEPRECATED
#  endif
#  define ATTRIBUTE_MALLOC
#  define ATTRIBUTE_NOINLINE
#  define ATTRIBUTE_NORETURN
#  define ATTRIBUTE_NOTHROW
#  define ATTRIBUTE_PRINTF(Format, FirstArg)
#  define ATTRIBUTE_PURE
#  define ATTRIBUTE_STDCALL
#  define ATTRIBUTE_UNUSED_RESULT
#  define ATTRIBUTE_USED

# endif
/*---------------------------.
| ATTRIBUTE_NOTHROW in C++.  |
`---------------------------*/

// For throw() vs. nothrow, see
// http://www.nabble.com/Rest-of-trivial-decorations-td23114765.html
# if defined __cplusplus
#  undef ATTRIBUTE_NOTHROW
#  define ATTRIBUTE_NOTHROW throw()
# endif


/*---------------------.
| ATTRIBUTE_COLD/HOT.  |
`---------------------*/

// Suggests the compiler that path executing the function annotated with one
// of the following attributes is either unlikely to be executed (cold) or
// likely to be executed (hot).  It is hard to determine which functions are
// hot, but cold function can be identified because they handle error,
// fallbacks and that they will never be called in a normal running process.


# if GCC_VERSION_GE(4, 3)
#  define ATTRIBUTE_COLD __attribute__((cold))
#  define ATTRIBUTE_HOT  __attribute__((hot))
# else
#  define ATTRIBUTE_COLD
#  define ATTRIBUTE_HOT
# endif



/*--------------.
| LIBPORT_NOP.  |
`--------------*/

// A no-op for macros that should expand to nothing, but not result in
// warnings such as "warning: empty body in an if-statement".

# define LIBPORT_NOP                            \
  ((void) 0)



/*------------------.
| LIBPORT_SECTION.  |
`------------------*/

// Each section must be pre-declared with its access rights, to do so,
// include compiler-section.hh.
# if defined _MSC_VER
#  define LIBPORT_SECTION(Name) __declspec(allocate(#Name))
# else
#  define LIBPORT_SECTION(Name) __attribute__((section(#Name)))
# endif



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


/*-------------------.
| likely, unlikely.  |
`-------------------*/

// Instrumentation of conditionnal values. (hand made profiling)
//
// if (unlikely(condition))
// {
//   // Handle fallback, errors and this will never be executed in a
//   // normal running process.
// }

# if GCC_VERSION_GE(4, 3)
#  define unlikely(expr) __builtin_expect (!!(expr), 0)
#  define likely(expr) __builtin_expect (!!(expr), 1)
# else
#  define unlikely(expr) expr
#  define likely(expr) expr
# endif


/*----------------------.
| __PRETTY_FUNCTION__.  |
`----------------------*/

// __PRETTY_FUNCTION__ is a GNU extension.  MSVC has something somewhat
// similar although it's less pretty.
# if defined _MSC_VER
#  define __PRETTY_FUNCTION__ __FUNCTION__
# endif // _MSC_VER


#endif // !LIBPORT_COMPILER_HH
