/**
 ** \file network/boost/server_ssl.hh
 ** \brief Boost.ASIO URBI server (SSL).
 */

#ifndef NETWORK_BOOST_SERVER_SSL_HH
# define NETWORK_BOOST_SERVER_SSL_HH
# include <boost/asio/ssl.hpp>
# include <network/boost/server.hh>
# include <network/boost/connection_ssl.hh>

namespace urbi
{
  namespace network
  {
    /// TCP server with optional SSL encryption.
    class ServerSSL : public Server<ConnectionSSL>
    {
    public:
      /// Parent type.
      typedef Server<ConnectionSSL> parent_t;
      /// SSL Socket type.
      typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_t;

      /// Ctor & dtor.
      /// \{
      explicit ServerSSL (const std::string& address,
                          int port,
                          std::size_t threadPoolSize) throw ();
      virtual ~ServerSSL () throw ();
      /// \}

      /// Password callback.
      std::string get_password() const throw ();

      boost::asio::ssl::context context_;
    protected:
      /// Handle completion of an asynchronous accept operation.
      virtual void handleAccept (const boost::system::error_code& e) throw ();
    };
  }; // end of namespace network.
}; // end of namespace urbi.

#endif // !NETWORK_BOOST_SERVER_SSL_HH
