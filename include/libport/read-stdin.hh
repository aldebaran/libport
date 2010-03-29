/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
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
  /// Read available data from \a fd without blocking.
  /// \throw libport::Exception on error, including EOF.
  LIBPORT_API std::string read_fd(int fd);

  /// Read available data from stdin without blocking.
  /// \throw libport::Exception on error, including EOF.
  LIBPORT_API std::string read_stdin();

  /// Return the whole content of \a file.
  /// \param file special case for "/dev/stdin".
  LIBPORT_API std::string read_file(const std::string& file);
}
#endif // !LIBPORT_READ_STDIN_HH
