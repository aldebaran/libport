#include <stdexcept>
#include <libport/network.h>
#include <libport/compiler.hh>

#include <network/bsdnet/connection.hh>

#include <kernel/userver.hh>

// Mac OSX does not have MSG_NOSIGNAL, used by send and recv to ask
// for events to become errno rather than signals.  But it supports
// the socket option SO_NOSIGPIPE.
#if !defined MSG_NOSIGNAL
# define MSG_NOSIGNAL 0
#endif


//! LinuxConnection constructor.
/*! The constructor calls UConnection::UConnection with the appropriate
 parameters.
 The global variable ::linuxserver saves the need to pass a UServer parameter
 to the LinuxConnection constructor.
 */
Connection::Connection(int fd)
  : UConnection (*::urbiserver, Connection::PACKETSIZE),
    fd(fd)
{
  // Test the error from UConnection constructor.
  if (uerror_ != USUCCESS)
    close();
  else
    initialize();
}

//! Connection destructor.
Connection::~Connection()
{
  if (fd != -1)
    close();
}

std::ostream&
Connection::dump (std::ostream& o) const
{
  return o
    << "Connection "
    << "{ controlFd = " << controlFd
    << ", fd = " << fd
    << " }";

}

//! Close the connection
/*!
 */
UConnection&
Connection::close()
{
  // Setting 'closing' to true tell the kernel not to use the
  // connection any longer.
  closing_ = true;

  // FIXME: Akim added those two lines, but he's not too sure
  // about them: should they be before "closing = true"?
  if (fd == -1)
    // We are already closed.
    CONN_ERR_RET(USUCCESS);
#if defined WIN32 && !defined __MINGW32__
  closesocket(fd);
  int ret = 0;//WSACleanup(); //wsastartup called only once!
#else
  int ret = ::close(fd);
  if (ret)
    perror ("cannot close connection fd");
#endif
  if (!ret)
    fd = -1;
  Network::unregisterNetworkPipe(this);
  if (ret)
    ERR_SET(UFAIL);
  else
  {
    fd = -1;
    ERR_SET(USUCCESS);
  }
  // This will call the dtor of this.
  server_.connection_remove(this);
  return *this;
}

void
Connection::doRead()
{
  int n = ::recv(fd, read_buff, PACKETSIZE, MSG_NOSIGNAL);
  if (n <= 0)
  {
    // No error message for clean connection termination.
    if (n)
      perror ("cannot recv");
    close();
    // Caught by Network::selectAndProcess inner loop.
    throw std::runtime_error("connection closed");
  }
  else
    this->received(read_buff, n);
}

size_t
Connection::effective_send (const char* buffer, size_t length)
{
  ECHO("Sending: " << std::string(buffer, length));
  int res = ::send(fd, buffer, length, MSG_NOSIGNAL);
  if (res == -1)
  {
    perror ("cannot send");
    close();
  }

  return res;
}

void
Connection::doWrite()
{
  continue_send();
}

UConnection&
Connection::send(const char* buffer, int length)
{
  if (send_queue_empty())
    trigger();
  return send_queue(buffer, length);
}

//! Send a "\n" through the connection
UConnection&
Connection::endline ()
{
  //FIXME: test send error
  UConnection::send("\n");
  CONN_ERR_RET(USUCCESS);
}
