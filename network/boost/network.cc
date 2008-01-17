/**
 ** \file network/boost/network.cc
 ** \brief Temporary glue.
 */
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "kernel/userver.hh"
#include "network/boost/network.hh"

namespace Network
{
  namespace
  {
    const int numThreads = 5;
    int port;
    std::string address;
    bool secured;
  };

  int
  createTCPServer (int p, char const* addr, bool sec)
  {
    port = p;
    if (!port)
      port = 54000 + rand () % 10000;
    address = addr;

#ifdef HAVE_OPENSSL
    secured = sec;
#else
    secured = false;
    if (sec)
      std::cerr
        << "Engine not compiled with SSL support, unsecured mode will be used."
        << std::endl;
#endif //HAVE_OPENSSL

    return port;
  }

#ifndef WIN32
  namespace
  {
    sigset_t old_mask;
  };

  void init ()
  {
    // Block all signals for background thread.
    sigset_t new_mask;
    sigfillset(&new_mask);
    pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);
  }

  void cleanup ()
  {
    // Restore previous signals.
    pthread_sigmask(SIG_SETMASK, &old_mask, 0);

    // Wait for signal indicating time to shut down.
    sigset_t wait_mask;
    sigemptyset(&wait_mask);
    sigaddset(&wait_mask, SIGINT);
    sigaddset(&wait_mask, SIGQUIT);
    sigaddset(&wait_mask, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &wait_mask, 0);
    int sig = 0;
    sigwait(&wait_mask, &sig);
  }
#else
  boost::function0<void> console_ctrl_function;

  BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
  {
    switch (ctrl_type)
    {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_SHUTDOWN_EVENT:
      console_ctrl_function();
      return TRUE;
    default:
      return FALSE;
    }
  }

  void init ()
  {
    console_ctrl_function = boost::bind(&urbi::network::Server::stop, &s);
    SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
  }

  void cleanup ()
  {
  }
#endif //!WIN32

  void
  startNetworkProcessingThread ()
  {
    if (secured)
      launchServer<serverSSL_t> ();
    else
      launchServer<server_t> ();

    //FIXME: needed in order to stop the server.
    ::urbiserver->shutdown ();
  }
};

