/*
 * Copyright (C) 2010, 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/echo.hh
/// \brief Obsolete, don't use.  Will be removed.  Use debug.hh.

#ifndef BUILDING_LIBPORT
# warning "use <libport.debug> instead"
#endif

#ifndef LIBPORT_ECHO_HH
# define LIBPORT_ECHO_HH

# include <iostream>
# include <string>
# include <libport/export.hh>

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

# include <libport/echo.hxx>

#endif
