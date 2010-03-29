/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_BOOST_ERROR_HXX
# define LIBPORT_BOOST_ERROR_HXX

# include <iostream>

namespace libport
{

  void boost_error(const std::string& s, boost::system::error_code erc)
  {
    std::cerr << s << ": " << erc.message() << std::endl;
  }

}

#endif // !LIBPORT_BOOST_ERROR_HXX
