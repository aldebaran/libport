// Network API based on boost::asio
#ifndef LIBPORT_ASIO_HH
# define LIBPORT_ASIO_HH

# include <iostream>
# include <boost/asio.hpp>
#ifndef LIBPORT_NO_SSL
# include <boost/asio/ssl.hpp>
#endif
# include <boost/function.hpp>

# include <libport/finally.hh>

namespace libport
{

  /** BaseSocket class.
   *
   * This class has a callback-based API: onReadFunc() and onErrorFunc().
   * The other boost::function members are filled-in by the underlying
   * implementation.
   */
  class BaseSocket
  {
    public:
      BaseSocket() :connected_(false){}
      virtual ~BaseSocket(){}
      libport::Finally deletor;
      /// Write data to the socket.
      boost::function2<void, void*, int> write;
      /// Connect to a remote host.
      inline  boost::system::error_code connect(const std::string& host,
	  const std::string& port, bool udp);
      /// Alias on write() for API compatibility.
      inline void send(void* addr, int len) {write(addr, len);}
      /// Alias on close() for API compatibility.
      inline void disconnect() {close();}
      /// Return if the socket is connected to a remote host.
      bool isConnected() { return connected_;}
      /// Disconnect the socket from the remote host.
      boost::function0<void> close;
      /// Callback function called each time new data is available.
      boost::function1<bool, boost::asio::streambuf&> onReadFunc;
      /// Callback function called in case of error on the socket.
      boost::function1<void, boost::system::error_code> onErrorFunc;
      /// Ask for the asynchronous deletion of this object.
      inline void destroy();
    public: // FIXME: !!!
      bool connected_;
  };
  /** Socket class with a higher API.
   *
   */
  class Socket: public BaseSocket
  {
    public:
      /** Called each time new data is received.
       *   \return the number of bytes used in buffer. The remaining data will
       *   be passed again to this function as soon as at least an extra byte
       *   is available.
       */
      virtual int onRead(const void*, int length){return length;}
      /** Called in case of error on the socket.
      */
      virtual void onError(boost::system::error_code){}
      inline Socket();
    protected:
      bool onRead_(boost::asio::streambuf&);
      std::string buffer;
  };

  /// FIXME: implement a way to stop listening.
  //typedef boost::shared_ptr<libport::Finally> Handle;

  typedef void* Handle;

  typedef boost::function0<BaseSocket*> SocketFactory;

  /** Listen on interface \b host, port \b port. Create a new BaseSocket for
   *  each new connection using the factory \b f. 
   */
  inline Handle listen(SocketFactory f, const std::string& host, int port, bool udp);

# ifndef LIBPORT_NO_SSL
  /// Same as listen(), but wrap the connection in a SSL layer.
  inline Handle listenSSL(SocketFactory f, const std::string& host, int port,
	boost::asio::ssl::context_base::method method
                   = boost::asio::ssl::context::sslv23_server,
        boost::asio::ssl::context::options options
                   = boost::asio::ssl::context::verify_none,
	const std::string& privateKeyFile="",
	const std::string& certChainFile="",
	const std::string& tmpDHFile="",
	const std::string& cipherList="");
# endif
}
# include "libport/asio.hxx"

#endif
