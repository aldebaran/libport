/// \file libport/compiler.hh
/// \brief Specific features from some compilers.

#ifndef LIBPORT_COMPILER_HH
# define LIBPORT_COMPILER_HH

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
#  endif
# endif


/*--------.
| SLEEP.  |
`--------*/

# ifdef LIBPORT_URBI_ENV_AIBO

// On aibo, make sure the log message arrives.

#  define SLEEP(C)					\
  do {							\
    long sum = 0;					\
    for (long i = 0; i < C * 100000000; ++i)		\
      sum += i;						\
  } while(0)

# else
#  define SLEEP(C) ((void) 0)
# endif // !LIBPORT_URBI_ENV_AIBO

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

    std::ostream& dump (std::ostream& o) const;
    static bool colored();
    std::string file_;
    unsigned line_;
    std::string function_;
  };

  inline
  std::ostream&
  operator<<(std::ostream& o, const EchoPrologue& e)
  {
    return e.dump(o);
  }
}

// We used to use __PRETTY_FUNCTION__, but this is really too verbose.
#  define LIBPORT_ECHO(Msg)                                     \
  do {                                                          \
    bool debug = !getenv("DISABLE_DEBUG_TRACES");               \
    if (debug)                                                  \
    {                                                           \
      std::cerr << libport::EchoPrologue(__FILE__, __LINE__,    \
                                         __FUNCTION__)          \
                << Msg << std::endl;                            \
      SLEEP(1);                                                 \
    }                                                           \
  } while (0)


// Disable debug traces when compiling with NDEBUG.
# ifdef NDEBUG
#  undef ENABLE_DEBUG_TRACES
# endif

# undef ECHO
# ifdef ENABLE_DEBUG_TRACES
#  include <iostream>
#  define ECHO(Msg) LIBPORT_ECHO(Msg)
# else
#  define ECHO(Msg) (void) 0
# endif

# undef ECHO_CMD
# define ECHO_CMD(Cmd)						\
  do {								\
    ECHO("Running " << #Cmd << "..." << std::endl);		\
    Cmd;							\
    ECHO("Running " << #Cmd << "... done" << std::endl);	\
  } while (0)

# undef PING
# define PING()  ECHO("ping")

#endif // !LIBPORT_COMPILER_HH
