/**
 ** \file network/boost/connection_ssl.hh
 ** \brief Boost.ASIO URBI session.
 */

#ifndef NETWORK_BOOST_CONNECTION_SSL_HH
# define NETWORK_BOOST_CONNECTION_SSL_HH
# include <boost/asio/ssl.hpp>
# include <boost/enable_shared_from_this.hpp>

# include "network/boost/connection.hh"

namespace urbi
{
  namespace network
  {
    class ConnectionSSL :
      public AbstractConnection<ConnectionSSL,
      boost::asio::ssl::stream<boost::asio::ip::tcp::socket> >
    {
    public:
      /// Socket type.
      typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_t;
      /// Parent type.
      typedef AbstractConnection<ConnectionSSL, socket_t> parent_t;

      explicit ConnectionSSL (boost::asio::io_service& ioService,
                              boost::asio::ssl::context& context) throw ();
      ~ConnectionSSL () throw ();

      void start ();
      void handleHandshake(const boost::system::error_code& error);
    private:
    };
  }; // end of namespace network.
}; // end of namespace urbi.

#endif // !NETWORK_BOOST_CONNECTION_SSL_HH
