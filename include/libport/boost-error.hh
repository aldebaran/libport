#ifndef LIBPORT_BOOST_ERROR_HH
# define LIBPORT_BOOST_ERROR_HH

# include <boost/system/error_code.hpp>
# include <string>

namespace libport
{

  void boost_error(const std::string& s, boost::system::error_code erc);

}

# include <libport/boost-error.hxx>

#endif // !LIBPORT_BOOST_ERROR_HH
