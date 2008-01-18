/**
 *  ** \file libport/read-stdin.hh
 *   ** \brief Declaration of read_stdin.
 *    **/

#ifndef LIBPORT_READ_STDIN_HH
# define LIBPORT_READ_STDIN_HH

# include "libport/config.h"

# include <string>

namespace libport
{
  /// Read available data from stdin without blocking.
  std::string read_stdin();
}
#endif
