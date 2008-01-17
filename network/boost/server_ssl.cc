/**
 ** \file network/boost/server_ssl.cc
 ** \brief Boost.ASIO URBI server (SSL).
 */

#include "network/boost/server_ssl.hh"

namespace urbi
{
  namespace network
  {
    ServerSSL::ServerSSL (const std::string& address,
                          int port,
                          std::size_t threadPoolSize) throw ()
      : parent_t (address, port, threadPoolSize, connectionPtr_t ()),
        context_ (ioService_, boost::asio::ssl::context::sslv23)
    {
      try
      {
        context_.set_options (
                              boost::asio::ssl::context::default_workarounds
                              | boost::asio::ssl::context::no_sslv2
                              | boost::asio::ssl::context::single_dh_use);

        // FIXME: the location of private key and encryption stuff should be
        // discussed.
        // Maybe some elements from the lock system may be re-used (?).
        context_.set_password_callback (boost::bind
                                        (&ServerSSL::get_password,
                                         this));
        context_.use_certificate_chain_file ("server.pem");
        context_.use_private_key_file ("server.pem",
                                       boost::asio::ssl::context::pem);
        context_.use_tmp_dh_file ("dh512.pem");

        newConnection_.reset (new connection_t (ioService_, context_));
        initialize (address, port, threadPoolSize);
      }
      catch (boost::system::system_error& e)
      {
        std::cerr << "Error during SSL initialization: " << e.what () << std::endl;
      }
    }

    ServerSSL::~ServerSSL () throw ()
    {
    }

    std::string
    ServerSSL::get_password() const throw ()
    {
      // FIXME: improve this.
      return "test";
    }

    void
    ServerSSL::handleAccept (const boost::system::error_code& error)
      throw ()
    {
      if (!error)
      {
        newConnection_->start ();
        newConnection_.reset (new connection_t (ioService_, context_));
        acceptor_.async_accept (newConnection_->socket (),
                                boost::bind
                                (&ServerSSL::handleAccept,
                                 this,
                                 boost::asio::placeholders::error));
        }
      else
        std::cerr << error.message () << std::endl;
    }
  }; // end of namespace network.
}; // end of namespace urbi.

