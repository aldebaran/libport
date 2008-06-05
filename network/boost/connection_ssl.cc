/**
 ** \file network/boost/connection_ssl.cc
 ** \brief Boost.ASIO URBI session.
 */

#include <boost/shared_ptr.hpp>

#include <network/boost/connection_ssl.hh>

namespace urbi
{
  namespace network
  {
    ConnectionSSL::ConnectionSSL (boost::asio::io_service& ioService,
                                  boost::asio::ssl::context& context)
      throw ()
      : parent_t (ioService, context)
    {
    }

    ConnectionSSL::~ConnectionSSL () throw ()
    {
    }

    void
    ConnectionSSL::start ()
    {
      socket_.async_handshake(boost::asio::ssl::stream_base::server,
                              strand_.wrap
                              (
                               boost::bind 
                               (
                                &ConnectionSSL::handleHandshake,
                                this->shared_from_this (),
                                boost::asio::placeholders::error)));
    }

    void
    ConnectionSSL::handleHandshake(const boost::system::error_code& error)
    {
      if (!error)
      {
        socket_.async_read_some
          (boost::asio::buffer (buffer_,
                               buffer_.size ()),
           strand_.wrap
           (
            boost::bind 
            (&ConnectionSSL::handleRead,
             this->shared_from_this (),
             boost::asio::placeholders::error,
             boost::asio::placeholders::bytes_transferred)));

        if (uerror_ != USUCCESS)
          closeConnection ();
        else
          initialize ();
      }
      else
        std::cerr << error.message () << std::endl;
    }

  }; // end of namespace network.
}; // end of namespace urbi.
