#ifndef NETWORK_BSDNET_CONNECTION_HH
# define NETWORK_BSDNET_CONNECTION_HH

/** \file Connection.h.cpp
 *  \brief the linux specialization of the UConnection class of the URBI kernel.
 *  @author Anthony Truchet from a previous work by Arnaud Sarthou  */

# include <sys/types.h>

# include <urbi/export.hh>
# include <kernel/utypes.hh>
# include <kernel/uconnection.hh>

# include <network/bsdnet/network.hh>

//! LinuxConnection implements an TCP/IP client connection.
class URBI_SDK_API Connection
  : public kernel::UConnection, public Network::Pipe
{
public:
  // Parameters used by the constructor.
  enum
  {
    MINSENDBUFFERSIZE = 4096,
    MAXSENDBUFFERSIZE = 33554432,
    // This is also the size of the buffer
    PACKETSIZE	     = 16384,
    MINRECVBUFFERSIZE = 4096,
    MAXRECVBUFFERSIZE = 33554432,
  };

  //! Creates a new connection from the connection file descriptor
  /**
   * @param fd the file descriptor of the underlying socket
   * @param clientinfo a pointer to the informations about the client connected
   * NOTE : the LinuxConnection stole the property of the struct hostent and
   * is thus responsible for its deletion, and the declaration of its allocation*/
  Connection(int connfd, const std::string& remote_ip);
  virtual ~Connection();
  virtual void close();

  virtual std::ostream& dump(std::ostream& o) const;

  /*ENABLE_URBI_MEM*/

  //! Called when the underlying fd is ready to be read
  void doRead();
  //! Called when the underlying fd is ready to be written
  void doWrite();

  virtual void notifyRead()
  {
    doRead();
  }
  virtual void notifyWrite()
  {
    doWrite();
  }
  virtual void send(const char* buffer, int length);

public:
  //! Accessor for the underlying file descriptor
  inline operator int() const
  {
    return fd;
  }

  virtual int readFD()
  {
    return fd;
  }

  virtual int writeFD()
  {
    return send_queue_empty() ? -1 : fd;
  }

  std::string getRemoteIP() const
  {
    return remote_ip_;
  }

protected:
  //! Overloading this function is required by UConnection
  virtual size_t effective_send(const char* buffer, size_t length);
  //! The file descriptor of the connection
  int fd;
  //! IP address of the remote host.
  std::string remote_ip_;
  //! The reception buffer
  char read_buff[PACKETSIZE];

public:
  virtual void endline();
};

#endif // !NETWORK_BSDNET_CONNECTION_HH
