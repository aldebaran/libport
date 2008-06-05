/**
 ** \file network/boost/network.hh
 ** \brief Temporary glue.
 */

#ifndef NETWORK_BOOST_NETWORK_HH
# define NETWORK_BOOST_NETWORK_HH
# include <string>
# include <network/boost/fwd.hh>
# include <network/boost/server.hh>

# ifdef HAVE_OPENSSL
# include <network/boost/server_ssl.hh>
# endif //HAVE_OPENSSL

namespace Network
{
  typedef urbi::network::Server<> server_t;

# ifdef HAVE_OPENSSL
  typedef urbi::network::ServerSSL serverSSL_t;
# else
  typedef urbi::network::Server<> serverSSL_t;
# endif //!HAVE_OPENSSL

  namespace
  {
    extern const int numThreads;
    extern int port;
    extern std::string address;
    extern bool secured;
  };

  int createTCPServer (int port, char const* address, bool secured = false);

  void init ();
  void cleanup ();

  template <typename Server>
  void launchServer ();

  void  startNetworkProcessingThread ();
};

# include <network/boost/network.hxx>
#endif // !NETWORK_BOOST_NETWORK_HH
