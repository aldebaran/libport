/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_BOOST_ERROR_HH
# define LIBPORT_BOOST_ERROR_HH

# include <boost/system/error_code.hpp>
# include <string>

namespace libport
{

  /// Same as perror, but for Boost errors:
  /// Display \a s, then the human readable message for \a erc.
  void boost_error(const std::string& s, boost::system::error_code erc);

}

# include <libport/boost-error.hxx>

#endif // !LIBPORT_BOOST_ERROR_HH
