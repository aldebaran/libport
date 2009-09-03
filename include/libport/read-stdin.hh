/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
/**
 ** \file libport/read-stdin.hh
 ** \brief Declaration of read_stdin.
 **/

#ifndef LIBPORT_READ_STDIN_HH
# define LIBPORT_READ_STDIN_HH

# include <string>
# include <libport/export.hh>

namespace libport
{
  /*! Read available data from stdin without blocking.
   *  Throws exception::Exception on error.
   */
  LIBPORT_API std::string read_stdin();
}
#endif // !LIBPORT_READ_STDIN_HH
