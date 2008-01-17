/**
 ** \file network/boost/server.cc
 ** \brief Boost.ASIO URBI server.
 */
#ifndef NETWORK_BOOST_SERVER_HXX
# define NETWORK_BOOST_SERVER_HXX
# include <boost/bind.hpp>
# include <boost/thread.hpp>
# include <boost/shared_ptr.hpp>

# include <boost/lexical_cast.hpp>

# include "network/boost/fwd.hh"
# include "network/boost/connection.hh"

//FIXME: remove me.
# include "network/boost/connection_ssl.hh"

namespace urbi
{
  namespace network
  {
    template <typename Connection>
    Server<Connection>::Server (const std::string& address,
                                int port,
                                std::size_t threadPoolSize) throw ()
      : threadPoolSize_ (threadPoolSize),
        ioService_ (),
        acceptor_ (ioService_),
        newConnection_ (new connection_t (ioService_))
    {
      initialize (address, port, threadPoolSize);
    }

    template <typename Connection>
    Server<Connection>::Server (const std::string&,
                                int,
                                std::size_t threadPoolSize,
                                connectionPtr_t ptr) throw ()
      : threadPoolSize_ (threadPoolSize),
        ioService_ (),
        acceptor_ (ioService_),
        newConnection_ (ptr)
    {
    }

    template <typename Connection>
    Server<Connection>::~Server () throw ()
    {
    }

    template <typename Connection>
    void
    Server<Connection>::initialize (const std::string& address,
                                    int port,
                                    std::size_t) throw ()
    {
      try
      {
        // Open the acceptor with the option to reuse the address
        // (i.e. SO_REUSEADDR).
        boost::asio::ip::tcp::resolver resolver (ioService_);

        std::string port_ = boost::lexical_cast<std::string> (port);
        boost::asio::ip::tcp::resolver::query query (address, port_);

        boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve (query);

        acceptor_.open (endpoint.protocol ());
        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address
                             (true));
        acceptor_.bind (endpoint);
        acceptor_.listen ();
        acceptor_.async_accept(newConnection_->socket (),
                               boost::bind
                               (&Server<Connection>::handleAccept, this,
                                boost::asio::placeholders::error));
      }
      catch (boost::system::system_error& e)
      {
        std::cerr << "Error while network initialization: " << e.what () << std::endl;
      }
    }

    template <typename Connection>
    void
    Server<Connection>::run() throw ()
    {
      typedef boost::shared_ptr<boost::thread> thread_t;

      // Create a pool of threads to run all of the io_services.
      std::vector<boost::shared_ptr<boost::thread> > threads;
      for (std::size_t i = 0; i < threadPoolSize_; ++i)
      {
        thread_t thread (new boost::thread
                         (boost::bind
                          (&boost::asio::io_service::run, &ioService_)));
        threads.push_back (thread);
      }

      // Wait for all threads in the pool to exit.
      for (std::size_t i = 0; i < threads.size (); ++i)
        threads[i]->join ();
    }

    template <typename Connection>
    void
    Server<Connection>::stop() throw ()
    {
      ioService_.stop ();
    }

    // FIXME: is it the right place?
    template <>
    void
    Server<ConnectionSSL>::handleAccept (const boost::system::error_code&)
      throw ();

    template <typename Connection>
    void
    Server<Connection>::handleAccept (const boost::system::error_code& error)
      throw ()
    {
      if (!error)
      {
        newConnection_->start ();
        newConnection_.reset (new connection_t (ioService_));
        acceptor_.async_accept (newConnection_->socket (),
                                boost::bind
                                (
                                 &Server<Connection>::handleAccept,
                                 this,
                                 boost::asio::placeholders::error));
      }
      else
        std::cerr << error.message () << std::endl;
    }

  }; // end of namespace network.
}; // end of namespace urbi.

#endif // !NETWORK_BOOST_SERVER_HXX
