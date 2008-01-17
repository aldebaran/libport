/**
 ** \file network/boost/server.hh
 ** \brief Boost.ASIO URBI server.
 */

#ifndef NETWORK_BOOST_SERVER_HH
# define NETWORK_BOOST_SERVER_HH
# include <boost/asio.hpp>
# include <boost/noncopyable.hpp>
# include <boost/shared_ptr.hpp>

# include "network/boost/fwd.hh"

namespace urbi
{
  namespace network
  {
    /// TCP server.
    template <typename Connection>
    class Server : private boost::noncopyable
    {
    public:
      /// Connection type.
      typedef Connection connection_t;
      typedef boost::shared_ptr<connection_t> connectionPtr_t;

      /// Ctor & dtor.
      /// \{

      /// Construct the server to listen on the specified TCP address
      /// and port.
      explicit Server (const std::string& address,
                       int port,
                       std::size_t threadPoolSize) throw ();

      /// Constructor used by ServerSSL.
      explicit Server (const std::string& address,
                       int port,
                       std::size_t threadPoolSize,
                       connectionPtr_t ptr) throw ();

      virtual ~Server () throw ();
      /// \}

      void
      initialize (const std::string& address,
                  int port,
                  std::size_t threadPoolSize) throw ();

      /// Run the server's io_service loop.
      void run () throw ();

      /// Stop the server.
      void stop () throw ();

      /// Get a new connection.
      connection_t* getNewConnection () throw ();

      /// Handle completion of an asynchronous accept operation.
      virtual void handleAccept (const boost::system::error_code& e) throw ();

      /// The number of threads that will call io_service::run().
      std::size_t threadPoolSize_;

      /// The io_service used to perform asynchronous operations.
      boost::asio::io_service ioService_;

      /// Acceptor used to listen for incoming connections.
      boost::asio::ip::tcp::acceptor acceptor_;

      /// The next connection to be accepted.
      connectionPtr_t newConnection_;
    };

  }; // end of namespace network.
}; // end of namespace urbi.

# include "network/boost/server.hxx"
#endif // !NETWORK_BOOST_SERVER_HH
