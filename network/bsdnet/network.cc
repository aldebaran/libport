
// Must be first to avoid inclusion of windows.h which will include winsock.h
#include <network/bsdnet/network.hh>

#include <boost/thread.hpp>
#include <boost/foreach.hpp>
#include <libport/compiler.hh>
#include <libport/cstdio>

#include <libport/unistd.h>
#include <libport/singleton-ptr.hh>
#include <list>
#include <algorithm>

#include <network/bsdnet/network.hh>
#include <network/bsdnet/connection.hh>

#include <kernel/userver.hh>

namespace Network
{

  Pipe::~Pipe()
  {
  }

  /*----------------.
  | TCPServerPipe.  |
  `----------------*/

  class TCPServerPipe: public Pipe
  {
  public:
    TCPServerPipe();
    bool init(int port, const std::string & address);
    ~TCPServerPipe();

    virtual std::ostream& dump (std::ostream& o) const;

    virtual int readFD();
    virtual int writeFD();

    virtual void notifyWrite();
    virtual void notifyRead();

    int getLocalPort();
  private:
    int fd;
    int port;
  };

  TCPServerPipe::TCPServerPipe()
    : Pipe(), fd(-1), port(-1)
  {
  }

  std::ostream&
  TCPServerPipe::dump(std::ostream& o) const
  {
    return o
      << "TCPServerPipe "
      << "{ controlFd = " << controlFd
      << ", fd = " << fd
      << ", port = " << port
      << '}';
  }

  TCPServerPipe::~TCPServerPipe ()
  {
    if (fd != -1)
    {
      if (shutdown (fd, SHUT_RDWR))
	perror ("cannot shutdown socket");
      else if (close (fd))
	perror ("cannot close socket");
    }
  }

  bool
  TCPServerPipe::init(int p, const std::string& addr)
  {
    port = p;
#if defined WIN32
    // Initialize the socket API, even on Mingw.  See
    // <URL:http://www.mingw.org/MinGWiki/index.php/sockets>:
    //
    //    The first difference to be aware of when porting networking
    //    programs to Windows is that you must call WSAStartup() before
    //    using any sockets functions. If you forget to do this, all
    //    Winsock functions will fail when called.
    //
    WSADATA info;
    // Winsock 1.1.
    if (WSAStartup(MAKEWORD(1, 1), &info))
      return false;
#endif
    // Create the socket.
    fd = socket(AF_INET, SOCK_STREAM, 0);
    ECHO("Created socket: " << fd);
    if (fd == -1)
    {
      perror ("cannot create socket");
      return false;
    }

    // Set the REUSEADDR option to 1 to allow imediate reuse of the port.
    // set the REUSEADDR option to 1 to allow imediate reuse of the port
    int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes))
    {
      perror ("setsockopt failed");
      return false;
    }

    // Do not send a SIGPIPE, rather return EPIPE.  See the comment
    // for MSG_NOSIGNAL in connection.cc.
#if defined SO_NOSIGPIPE
    if (setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &yes, sizeof yes))
    {
      perror ("setsockopt failed");
      return false;
    }
#endif

    // Fill in socket address.
    sockaddr_in address;
    memset(&address, 0, sizeof (sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons((unsigned short) port);
    if (addr.empty() || addr == "0.0.0.0")
      address.sin_addr.s_addr = INADDR_ANY;
    else
    {
      //attempt name resolution
      hostent* hp = gethostbyname (addr.c_str());
      if (!hp) //assume IP address in case of failure
	// FIXME: Check that inet_addr did not return INADDR_NONE.
	address.sin_addr.s_addr = inet_addr (addr.c_str());
      else
      {
	/* hp->h_addr is now a char* such as the IP is:
	 *    a.b.c.d
	 * where
	 *    a = hp->h_addr[0]
	 *    b = hp->h_addr[1]
	 *    c = hp->h_addr[2]
	 *    d = hp->h_addr[3]
	 * hence the following calculation.  Don't cast this to an int*
	 * because of the alignment problems (eg: ARM) and also because
	 * sizeof (int) is not necessarily 4 and also because the result
	 * depends on the endianness of the host.
	 */
	memcpy (&address.sin_addr, hp->h_addr, hp->h_length);
      }
    }

    // Bind to port.
    if (bind(fd, (sockaddr*) &address, sizeof (sockaddr)) == -1)
    {
      perror ("cannot bind");
      return false;
    }

    // Listen for connections.
    if (listen(fd, 1))
    {
      perror ("cannot listen");
      return false;
    }

    registerNetworkPipe(this);
    return true;
  }


  int
  TCPServerPipe::readFD()
  {
    return fd;
  }

  int
  TCPServerPipe::writeFD()
  {
    return -1;
  }

  void
  TCPServerPipe::notifyRead()
  {
    sockaddr_in client;
    socklen_t asize = sizeof (sockaddr_in);
    int cfd = accept(fd, (sockaddr*) &client, &asize);
    if (cfd == -1)
    {
      perror ("cannot accept");
      return;
    }
    Connection* c = new Connection(cfd);
    ::urbiserver->connection_add(c);
    registerNetworkPipe(c);
  }

  void
  TCPServerPipe::notifyWrite()
  {
    // FIXME: It this really ok to ignore?
  }

  int
  TCPServerPipe::getLocalPort()
  {
    struct sockaddr_in address;
    socklen_t len = sizeof (struct sockaddr_in);
    getsockname(fd, (struct sockaddr *)&address, &len);
    return ntohs(address.sin_port);
  }

  /*-------------------------.
  | Freestanding functions.  |
  `-------------------------*/

  std::ostream&
  operator<< (std::ostream& o, const Pipe& p)
  {
    return p.dump (o);
  }

  int
  createTCPServer(int port, std::string address)
  {
    TCPServerPipe* tsp = new TCPServerPipe();
    if (!tsp->init(port, address))
    {
      delete tsp;
      return 0;
    }
    return tsp->getLocalPort();
  }

#if !defined WIN32
  int controlPipe[2] = { -1, -1 };
#endif
  typedef std::list<Pipe*> pipes_type;
};

// This is called out of any namespace due to vcxx error C2888
// cf: http://msdn.microsoft.com/en-us/library/27zksbks(VS.80).aspx
STATIC_INSTANCE_DECL_NS(pipes_type, pList, Network);

namespace Network
{

  int
  buildFD(fd_set& rd, fd_set& wr, fd_set &er)
  {
    FD_ZERO(&rd);
    FD_ZERO(&wr);
    FD_ZERO(&er);
    int maxfd = 0;
#if !defined WIN32
    LIBPORT_FD_SET(controlPipe[0], &rd);
    maxfd = controlPipe[0];
#endif
    BOOST_FOREACH(Pipe *p, *pList)
    {
      int f = p->readFD();
      if (f > 0)
      {
	LIBPORT_FD_SET(f, &rd);
	LIBPORT_FD_SET(f, &er);
      }
      if (f > maxfd)
	maxfd = f;
      int g = p->writeFD();
      if (g > 0)
	LIBPORT_FD_SET(g, &wr);
      if (g > maxfd)
	maxfd = g;
    }
    return maxfd + 1;
  }

  void notify(fd_set& rd, fd_set& wr, fd_set &er)
  {
    // We cannot use a simple loop with a single iterator here,
    // because calls to notifyRead or notifyWrite can call
    // unregisterNetworkPipe which changes pList, resulting in an
    // invalidated iterator.  So we do need two iterators to walk the
    // list.
    // Also, our pipes can delete themselves in notifyRead(), in which case they
    // must throw an exception.
    for (pipes_type::iterator i = pList->begin(); i != pList->end(); )
    {
      // Next iterator.
      pipes_type::iterator in = i;
      ++in;
      {
	try
	{
	  Pipe& p = **i;
	  int f = p.readFD();
	  if (f >= 0 && (LIBPORT_FD_ISSET(f, &rd) || LIBPORT_FD_ISSET(f, &er)))
	    p.notifyRead();

	  f = p.writeFD();
	  if (f >= 0 && LIBPORT_FD_ISSET(f, &wr))
	    p.notifyWrite();
	}
	catch(...)
	{
	  //this can happen if the object was destroyed by the notifyRead
	}
      }
      i = in;
    }
  }

  bool
  selectAndProcess(libport::utime_t usDelay)
  {
    fd_set rd;
    fd_set wr;
    fd_set er;
    int mx = buildFD(rd, wr, er);
    timeval tv;
    tv.tv_sec = usDelay / 1000000;
    tv.tv_usec = usDelay - ((usDelay / 1000000) * 1000000);
    int r = select(mx, &rd, &wr, &er, &tv);
    if (r < 0)
      // FIXME: This is bad, we should really do something.
      perror("cannot select");
    else if (!r)
      return false;
    else // 0 < r
    {
#if !defined WIN32
      if (LIBPORT_FD_ISSET(controlPipe[0], &rd))
      {
	char buf[128];
	if (read(controlPipe[0], buf, sizeof buf) == -1)
	  perror ("cannot read controlPipe[0]");
      }
#endif
      notify(rd, wr, er);
    }

    return r > 0;
  }


  void registerNetworkPipe(Pipe* p)
  {
    pList->push_back(p);
#if !defined WIN32
    if (controlPipe[0] == -1)
      pipe(controlPipe);
    p->controlFd = controlPipe[1];
#endif
  }

  void unregisterNetworkPipe(Pipe* p)
  {
    pList->remove(p);
  }

#if defined WIN32
  enum { delay = 10000 };
#else
  enum { delay = 1000000 };
#endif

  static void processNetwork()
  {
    while (true)
      selectAndProcess(delay);
  }


  void startNetworkProcessingThread()
  {
    boost::thread networkThread(&processNetwork);
  }



  /*-------.
  | Pipe.  |
  `-------*/

  Pipe::Pipe ()
    : controlFd (-1)
  {}

  void Pipe::trigger()
  {
#if !defined WIN32 || defined __MINGW32__
    char c = 0;
    if (write(controlFd, &c, 1) == -1)
      perror ("cannot write to controlFD");
#endif
  }
}
