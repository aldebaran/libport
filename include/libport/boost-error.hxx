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
