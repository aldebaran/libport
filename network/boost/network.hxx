/**
 ** \file network/boost/network.hxx
 ** \brief Temporary glue.
 */

#ifndef NETWORK_BOOST_NETWORK_HXX
# define NETWORK_BOOST_NETWORK_HXX

# include <exception>

namespace Network
{
  template <typename Server>
  void launchServer ()
  {
    try
    {
      init ();
      Server networkServer (address, port, numThreads);
      boost::thread t(boost::bind(&Server::run, &networkServer));
      cleanup ();

      // Stop the server.
      networkServer.stop();
      t.join();
    }
    catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;;
    }
  }
};

#endif // !NETWORK_BOOST_NETWORK_HXX
