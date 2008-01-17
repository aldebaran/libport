/**
 ** \file network/boost/server.cc
 ** \brief Boost.ASIO URBI server.
 */

# include "network/boost/server.hh"

namespace urbi
{
  namespace network
  {
    template <>
    void
    Server<ConnectionSSL>::handleAccept (const boost::system::error_code&)
      throw ()
    {
      // FIXME: berk.
      pabort ("should never happen.");
    }
  };
};
