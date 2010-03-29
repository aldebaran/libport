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

# include <iostream>
# include <string>
# include <libport/config.h>
# include <libport/export.hh>

/*----------------.
| __attribute__.  |
`----------------*/

# ifdef _MSC_VER

#  define __attribute__(a)
#  define ATTRIBUTE_ALWAYS_INLINE __forceinline
#  define ATTRIBUTE_DEPRECATED __declspec(deprecated)
#  define ATTRIBUTE_NOINLINE __declspec(noinline)
#  define ATTRIBUTE_NORETURN __declspec(noreturn)
#  define ATTRIBUTE_NOTHROW  __declspec(nothrow)
#  define ATTRIBUTE_UNUSED_RESULT /* FILLME */

# endif

# ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
#  if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 5) || __STRICT_ANSI__
#   define __attribute__(Spec) /* empty */
#  else
     // GCC 3.4 does not support "always_inline" without "inline".
#    define ATTRIBUTE_ALWAYS_INLINE inline __attribute__((__always_inline__))
#    define ATTRIBUTE_DEPRECATED __attribute__((__deprecated__))
#    define ATTRIBUTE_NOINLINE __attribute__((__noinline__))
#    define ATTRIBUTE_NORETURN __attribute__((__noreturn__))
#    define ATTRIBUTE_NOTHROW  __attribute__((__nothrow__))
#    define ATTRIBUTE_UNUSED_RESULT __attribute__((warn_unused_result))
#  endif
# endif

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
| ECHO & PING.  |
`--------------*/

namespace libport
{
  struct LIBPORT_API EchoPrologue
  {
    // Maximum length of file name prefix.
    enum { length = 30 };

    EchoPrologue(const std::string& file, unsigned line,
                 const std::string& function);

    std::ostream& dump(std::ostream& o) const;
    static bool colored();
    std::string file_;
    unsigned line_;
    std::string function_;
  };

  std::ostream& operator<<(std::ostream& o, const EchoPrologue& e);
}

// We used to use __PRETTY_FUNCTION__, but this is really too verbose.
#  define LIBPORT_ECHO_PROLOGUE                                 \
  libport::EchoPrologue(__FILE__, __LINE__, __FUNCTION__)

// Care was taken to make of LIBPORT_ECHO an expression, not a
// statement.  It returns std::cerr.
#  define LIBPORT_ECHO(Msg)                                             \
  (getenv("DISABLE_DEBUG_TRACES")                                       \
   ? std::cerr                                                          \
   : std::cerr << LIBPORT_ECHO_PROLOGUE << Msg << std::endl)

// Disable debug traces when compiling with NDEBUG.
# ifdef NDEBUG
#  undef ENABLE_DEBUG_TRACES
# endif

# ifdef ENABLE_DEBUG_TRACES
#  include <iostream>
#  define LIBPORT_DEBUG(Msg) LIBPORT_ECHO(Msg)
# else
#  define LIBPORT_DEBUG(Msg) (void) 0
# endif

# define LIBPORT_RUN(Cmd)                                               \
  do {                                                                  \
    LIBPORT_DEBUG("Running " << #Cmd << "..." << std::endl);		\
    Cmd;                                                                \
    LIBPORT_DEBUG("running " << #Cmd << "... done" << std::endl);	\
  } while (0)

# define LIBPORT_PING()  LIBPORT_DEBUG("ping")

# include <libport/compiler.hxx>

#endif // !LIBPORT_COMPILER_HH
