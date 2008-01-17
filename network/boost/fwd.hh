/**
 ** \file network/boost/fwd.hh
 ** \brief Forward declarations.
 */

#ifndef NETWORK_BOOST_FWD_HH
# define NETWORK_BOOST_FWD_HH
# include <boost/asio.hpp>

# define HAVE_OPENSSL

namespace urbi
{
  namespace network
  {
    template <typename Exact,
              typename Socket = boost::asio::ip::tcp::socket>
    class AbstractConnection;

    class Connection;

    template <typename C = Connection>
    class Server;

#ifdef HAVE_OPENSSL
    class ServerSSL;
    class ConnectionSSL;
#endif //!HAVE_OPENSSL

  }; // end of namespace network.
}; // end of namespace urbi.

#endif // !NETWORK_BOOST_FWD_HH
