/**
 ** \file network/boost/connection.hh
 ** \brief Boost.ASIO URBI session.
 */

#ifndef NETWORK_BOOST_CONNECTION_HH
# define NETWORK_BOOST_CONNECTION_HH

# include <boost/asio.hpp>
# include <boost/asio/ssl.hpp>
# include <boost/array.hpp>
# include <boost/noncopyable.hpp>
# include <boost/shared_ptr.hpp>
# include <boost/enable_shared_from_this.hpp>
# include <boost/thread.hpp>

# include "kernel/utypes.hh"
# include "kernel/uconnection.hh"

# include "network/boost/fwd.hh"

namespace urbi
{
  namespace network
  {
    // FIXME: split this file.

    /// Abstract connection which factorizes code.
    template <typename Exact, typename Socket>
    class AbstractConnection : public UConnection,
                               public boost::enable_shared_from_this<Exact>,
                               private boost::noncopyable
    {
    public:
      // Socket type.
      typedef Socket socket_t;

      /// Internal socket type.
      // This is used to know the socket type independently from the SSL layer.
      typedef typename socket_t::lowest_layer_type realSocket_t;

      // Parameters used by the constructor.
      enum
      {
        MINSENDBUFFERSIZE = 4096,
        MAXSENDBUFFERSIZE = 33554432,
        // This is also the size of the buffer
        PACKETSIZE	     = 16384,
        MINRECVBUFFERSIZE = 4096,
        MAXRECVBUFFERSIZE = 33554432,
      };

      /// Ctor & dtor.
      /// \{

      /// Construct a connection with the given io_service.
      explicit AbstractConnection (boost::asio::io_service& io_service)
        throw ();


      ~AbstractConnection () throw ();
      /// \}

      UConnection& closeConnection () throw ();

      /// Get the socket associated with the connection.
      realSocket_t& socket() throw ();

      /// Start the first asynchronous operation for the connection.
      void start() throw ();

      /// Send a \n through the socket.
      virtual UConnection& endline () throw ();

    protected:
      /// Constructor used by ConnectionSSL.
      explicit AbstractConnection (boost::asio::io_service& io_service,
                                   boost::asio::ssl::context& context)
        throw ();

      /// Send a buffer through the socket.
      virtual int effectiveSend (const ubyte *buffer, int length) throw ();

      /// Handle completion of a read operation.
      void handleRead(const boost::system::error_code& e,
                      std::size_t bytes_transferred) throw ();

      /// Handle completion of a write operation.
      void handleWrite(const boost::system::error_code& e) throw ();

      /// Strand to ensure the connection's handlers are not called
      /// concurrently.
      boost::asio::io_service::strand strand_;

      /// Socket for the connection.
      socket_t socket_;

      /// Buffer for incoming data.
      boost::array<char, PACKETSIZE> buffer_;
    };

    /// Represents a single connection from a client.
    /// (no SSL)
    class Connection : public AbstractConnection<Connection>
    {
    public:
      explicit Connection (boost::asio::io_service& io_service)
        throw ()
        : AbstractConnection<Connection> (io_service)
      {}
    };

  }; // end of namespace network.
}; // end of namespace urbi.

# include "network/boost/connection.hxx"
#endif // !NETWORK_BOOST_CONNECTION_HH
