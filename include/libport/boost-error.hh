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
