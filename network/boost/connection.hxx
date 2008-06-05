/**
 ** \file network/boost/connection.cc
 ** \brief Boost.ASIO URBI session.
 */
#ifndef NETWORK_BOOST_CONNECTION_HXX
# define NETWORK_BOOST_CONNECTION_HXX
# include <boost/bind.hpp>
# include <kernel/userver.hh>

namespace urbi
{
  namespace network
  {

    template <typename Exact, typename Socket>
    AbstractConnection<Exact, Socket>::AbstractConnection
    (boost::asio::io_service& ioService) throw ()
      : UConnection (::urbiserver,
                     AbstractConnection::MINSENDBUFFERSIZE,
                     AbstractConnection::MAXSENDBUFFERSIZE,
                     AbstractConnection::PACKETSIZE,
                     AbstractConnection::MINRECVBUFFERSIZE,
                     AbstractConnection::MAXRECVBUFFERSIZE),
        strand_ (ioService),
        socket_ (ioService),
        buffer_ ()
    {
    }

    // FIXME: this introduces SSL in this class, this should be changed if
    // it prevents from compiling if OpenSSL is not available.
    template <typename Exact, typename Socket>
    AbstractConnection<Exact, Socket>::AbstractConnection
    (boost::asio::io_service& ioService,
     boost::asio::ssl::context& context) throw ()
      : UConnection (::urbiserver,
                     AbstractConnection::MINSENDBUFFERSIZE,
                     AbstractConnection::MAXSENDBUFFERSIZE,
                     AbstractConnection::PACKETSIZE,
                     AbstractConnection::MINRECVBUFFERSIZE,
                     AbstractConnection::MAXRECVBUFFERSIZE),
        strand_ (ioService),
        socket_ (ioService, context),
        buffer_ ()
    {
    }

    template <typename Exact, typename Socket>
    AbstractConnection<Exact, Socket>::~AbstractConnection () throw ()
    {
    }

    template <typename Exact, typename Socket>
    UConnection&
    AbstractConnection<Exact, Socket>::closeConnection () throw ()
    {
      // Initiate graceful connection closure.
      closing = true;
      boost::system::error_code ignored_ec;
      socket ().shutdown
        (boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
      return *this;
    }

    template <typename Exact, typename Socket>
    typename AbstractConnection<Exact, Socket>::realSocket_t&
    AbstractConnection<Exact, Socket>::socket () throw ()
    {
      return socket_.lowest_layer ();
    }

    template <typename Exact, typename Socket>
    void
    AbstractConnection<Exact, Socket>::start () throw ()
    {
      socket_.async_read_some
        (boost::asio::buffer (buffer_),
         strand_.wrap
         (
          boost::bind
          (
           &AbstractConnection<Exact, Socket>::handleRead,
           this->shared_from_this (),
           boost::asio::placeholders::error,
           boost::asio::placeholders::bytes_transferred)));

      if (uerror_ != USUCCESS)
        closeConnection ();
      else
        initialize ();
    }

    template <typename Exact, typename Socket>
    UConnection&
    AbstractConnection<Exact, Socket>::endline () throw ()
    {
      //FIXME: test send error
      (*this) << UConnection::send((const ubyte*)"\n", 1);
      CONN_ERR_RET(USUCCESS);
    }

    template <typename Exact, typename Socket>
    int
    AbstractConnection<Exact, Socket>::effectiveSend
    (const ubyte *buffer, int length) throw ()
    {
      boost::asio::async_write
        (socket_,
         boost::asio::buffer (reinterpret_cast<const char *> (buffer)),
         strand_.wrap (
                       boost::bind
                       (
                        &AbstractConnection<Exact, Socket>::handleWrite,
                        this->shared_from_this (),
                        boost::asio::placeholders::error)));
      // FIXME: this may produce bugs.
      return length;
    }

    template <typename Exact, typename Socket>
    void
    AbstractConnection<Exact, Socket>::handleRead
    (const boost::system::error_code& error,
     std::size_t bytes_transferred) throw ()
    {
      if (!error)
      {
        (*this) << UConnection::received ((const ubyte*)&buffer_,
                                          bytes_transferred);

        socket_.async_read_some
          (
           boost::asio::buffer (buffer_),
           strand_.wrap (
                         boost::bind
                         (
                          &AbstractConnection<Exact, Socket>::handleRead,
                          this->shared_from_this (),
                          boost::asio::placeholders::error,
                          boost::asio::placeholders::bytes_transferred)));
      }
      else
        std::cerr << error.message () << std::endl;

      // If an error occurs then no new asynchronous operations are started.
      // This means that all shared_ptr references to the connection object
      // will disappear and the object will be destroyed automatically after
      // this handler returns. The connection class's destructor closes the
      // socket.
    }

    template <typename Exact, typename Socket>
    void AbstractConnection<Exact, Socket>::handleWrite
    (const boost::system::error_code& error) throw ()
    {
      if (error)
      {
        std::cerr << error.message () << std::endl;
        closeConnection ();
      }

      // No new asynchronous operations are started. This means that all
      // shared_ptr references to the connection object will disappear and
      // the object will be destroyed automatically after this handler
      // returns. The connection class's destructor closes the socket.
    }

  }; // end of namespace network.
}; // end of namespace urbi.

#endif // !NETWORK_BOOST_CONNECTION_HXX
