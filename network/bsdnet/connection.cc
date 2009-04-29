#include <stdexcept>
#include <libport/network.h>
#include <libport/compiler.hh>

#include <network/bsdnet/connection.hh>

#include <kernel/userver.hh>
#include <object/cxx-conversions.hh>
#include <object/lobby.hh>
#include <object/symbols.hh>

//! LinuxConnection constructor.
/*! The constructor calls UConnection::UConnection with the appropriate
 parameters.
 The global variable ::linuxserver saves the need to pass a UServer parameter
 to the LinuxConnection constructor.
 */
Connection::Connection(int fd, const std::string& remote_ip)
  : UConnection(*kernel::urbiserver, Connection::PACKETSIZE)
  , fd(fd)
  , remote_ip_(remote_ip)
{
  // Test the error from UConnection constructor.
  if (uerror_ != USUCCESS)
    close();
  else
    initialize();
  lobby_->slot_set(SYMBOL(remoteIP), object::to_urbi(remote_ip_));
}

//! Connection destructor.
Connection::~Connection()
{
  if (fd != -1)
    close();
}

std::ostream&
Connection::dump(std::ostream& o) const
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
void
Connection::close()
{
  if (!closing_)
  {
    closing_ = true;
    Network::unregisterNetworkPipe(this);
  }

  // FIXME: Akim added those two lines, but he's not too sure
  // about them: should they be before "closing = true"?
  if (fd == -1)
  {
    // We are already closed.
    error_ = USUCCESS;
    return;
  }
#if defined WIN32 && !defined __MINGW32__
  closesocket(fd);
  int ret = 0;//WSACleanup(); //wsastartup called only once!
#else
  int ret = ::close(fd);
  if (ret)
    perror("cannot close connection fd");
#endif
  if (!ret)
    fd = -1;
  if (ret)
    error_ = UFAIL;
  else
  {
    fd = -1;
    error_ = USUCCESS;
  }
  // This will call the dtor of this.
  server_.connection_remove(this);
}

void
Connection::doRead()
{
  int n = ::recv(fd, read_buff, PACKETSIZE, MSG_NOSIGNAL);
  if (n <= 0)
  {
    // No error message for clean connection termination.
    if (n)
      perror("cannot recv");
    close();
    // Caught by Network::selectAndProcess inner loop.
    throw std::runtime_error("connection closed");
  }
  else
    this->received(read_buff, n);
}

size_t
Connection::effective_send(const char* buffer, size_t length)
{
  ECHO("Sending: " << std::string(buffer, length));
  int res = ::send(fd, buffer, length, MSG_NOSIGNAL);
  if (res == -1)
  {
    perror("cannot send");
    close();
    // Caught by Network::selectAndProcess inner loop.
    throw std::runtime_error("connection closed");
  }

  return res;
}

void
Connection::doWrite()
{
  continue_send();
}

void
Connection::send(const char* buffer, int length)
{
  if (send_queue_empty())
    trigger();
  send_queue(buffer, length);
}

//! Send a "\n" through the connection
void
Connection::endline()
{
  //FIXME: test send error
  UConnection::send("\n");
  error_ = USUCCESS;
}
