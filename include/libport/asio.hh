// Network API based on boost::asio
#ifndef LIBPORT_ASIO_HH
# define LIBPORT_ASIO_HH

# include <libport/config.h>
# include <libport/sys/socket.h>


# include <iostream>
# include <boost/asio.hpp>
#ifndef LIBPORT_NO_SSL
# include <boost/asio/ssl.hpp>
#endif
# include <boost/function.hpp>

# include <libport/destructible.hh>
# include <libport/finally.hh>
# include <libport/utime.hh>

namespace libport
{
  class AsioDestructible: public Destructible
  {
    protected:
      virtual void doDestroy();
  };

  /** BaseSocket class.
   *
   * This class has a callback-based API: onReadFunc() and onErrorFunc().
   */
  class BaseSocket: public AsioDestructible
  {
  public:
    virtual ~BaseSocket(){}
    libport::Finally deletor;
    /// Write data asynchronously to the socket.
    virtual void write(const void* data, unsigned int length) = 0;
    /// Alias on write() for API compatibility.
    inline void send(void* addr, size_t len) {write((const void*)addr, len);}
    /// Alias on close() for API compatibility.
    inline void disconnect() {close();}
    /// Return if the socket is connected to a remote host.
    virtual bool isConnected() = 0;
    /// Disconnect the socket from the remote host, calls onError.
    virtual void close() = 0;
    /// Get port of remote endpoint
    virtual unsigned short getRemotePort() = 0;
    /// Get IP address of remote endpoint
    virtual std::string getRemoteHost() = 0;
    /// Get port of local endpoint
    virtual unsigned short getLocalPort() = 0;
    /// Get IP address of local endpoint
    virtual std::string getLocalHost() = 0;
    virtual int stealFD() = 0;
    virtual int getFD() = 0;
    /// Callback function called each time new data is available.
    boost::function1<bool, boost::asio::streambuf&> onReadFunc;
    /// Callback function called in case of error on the socket.
    boost::function1<void, boost::system::error_code> onErrorFunc;
    /// Mutex to protect access to the above callbacks.
    Lockable callbackLock;
  };

  /// Endpoint on an UDP socket.
  class UDPLink
  {
  public:
    virtual ~UDPLink() {}
    virtual void reply(const void* data, size_t length) = 0;
    inline void reply(const std::string& s) {reply(s.c_str(), s.length());}
  };

  /** Socket class with a higher API.
   *
   * It is recommended that you always asynchronously destroy Socket instances
   * by calling the destroy() method. If you want to put instances in the stack,
   * or explicitly call delete for whatever reason, you must in your destructor:
   * - call close()
   * - call wasDestroyed()
   * - call waitForDestructionPermission(), if the io_service is running in
   *   another thread(the default behavior).
   */
  class Socket: public AsioDestructible
  {
  public:
    Socket(): base_(0){}
    virtual ~Socket();
    /** Set underlying BaseSocket object, setup its callbacks to call our virtual functions.
     */
    virtual void setBase(BaseSocket*);

    /** Called each time new data is received.
     *   \return the number of bytes used in buffer. The remaining data will
     *   be passed again to this function as soon as at least an extra byte
     *   is available.
     */
    virtual int onRead(const void*, size_t length){return length;}

    /** Called in case of error on the socket. By default, do nothing.
     */
    virtual void onError(boost::system::error_code);

    /** Called on socket connection
     */
     virtual void onConnect() {}

    /// Ask for the asynchronous destruction of this object.
    virtual void destroy();

    inline void write(const void* data, unsigned int length)
      { base_->write(data, length);}
    /// Alias on write() for API compatibility.
    inline void send(void* addr, size_t len) {write((const void*)addr, len);}
    inline void send(const std::string& s) {write(s.c_str(), s.length());}
    inline void close() {if (base_) base_->close();}
    inline unsigned short getRemotePort() { return base_->getRemotePort();}
    inline std::string getRemoteHost() {return base_->getRemoteHost();}
    inline unsigned short getLocalPort() { return base_->getLocalPort();}
    inline std::string getLocalHost() {return base_->getLocalHost();}
    inline bool isConnected() {return base_?base_->isConnected():false;}

    /** Connect to a remote host.
     * \param host hostname to connect to.
     * \param port port to connect to, as a service name or an int.
     * \param usTimeout timeout in microseconds, 0 meaning none.
     * \param asynchronous make an asynchronous attempt. The function will
     *    return immediately, and either onConnect will be called when the
     *    connection succeeds, or onError() will be called.
     * \return an error code if the connection failed.
     *
     */
    boost::system::error_code connect(const std::string& host,
                                      const std::string& port,
                                      bool udp=false, utime_t usTimeout = 0,
                                      bool asynchronous = false);

    /** Make an asynchronous connection attempt to a remote host.
     *  This API garantees that either the onConnect() or the onError() function
     *  will be called.
     * \param host hostname to connect to.
     * \param port port to connect to, as a service name or an int.
     * \param usTimeout timeout in microseconds, 0 meaning none.
     */
    void async_connect(const std::string& host,
                       const std::string& port,
                       bool udp=false, utime_t usTimeout = 0)
    {
      connect(host, port, udp, usTimeout, true);
    }


    typedef void* Handle;
    typedef boost::function0<Socket*> SocketFactory;
    /** Listen using udp.
     * Call onRead(data, length, link) for each new packet. Link can be used
     * to reply to the sender through its UDPLink::reply() method.
     */
    static Handle listenUDP(const std::string& host, const std::string& port,
                            boost::function3<void, const void*, int,
                            boost::shared_ptr<UDPLink> > onRead,
                            boost::system::error_code& erc);

    /** Listen using TCP or UDP.
     *
     * \param f a socket factory. For each new connection the function will
     * be called and the resulting socket object bound to the new connection.
     */
    boost::system::error_code listen(SocketFactory f, const std::string& host,
                                     const std::string& port, bool udp = false);
    boost::system::error_code listen(SocketFactory f, const std::string& host,
                                     unsigned port, bool udp = false);

#ifndef LIBPORT_NO_SSL
    boost::system::error_code
    listenSSL(SocketFactory f,
              const std::string& host,
              const std::string&  port,
              boost::asio::ssl::context_base::method ctx
              = boost::asio::ssl::context::sslv23_server,
              boost::asio::ssl::context::options options
              = boost::asio::ssl::context::verify_none,
              const std::string& privateKeyFile = "",
              const std::string& certChainFile = "",
              const std::string& tmpDHFile = "" ,
              const std::string& cipherList = "");
#endif

    /// Steal file descriptor from Socket. Return the file descriptor.
    int stealFD();
    /// Get file descriptor from Socket.
    int getFD();
    /// Set file descriptor
    template<class Sock>
    void setFD(int fd, typename Sock::protocol_type proto);
  protected:
    bool onRead_(boost::asio::streambuf&);
    std::string buffer;
    BaseSocket* base_;
  private:
    template<typename Proto, typename BaseFactory> boost::system::error_code
    listenProto(SocketFactory f, const std::string& host,
                const std::string&port, BaseFactory bf);
    template<typename Proto, typename BaseFactory> boost::system::error_code
    connectProto(const std::string& host, const std::string& port,
                 utime_t timeout, bool async, BaseFactory bf);
  };

  //FIXME: extend to provide a way to ensure workerThread not started.
  /** Get the io_service handling all asynchronous operations.
   *
   * \param startWorkerThread create a thread in which to run the io_service.
   * If \b startWorkerThread is false on \b first invocation, the user
   * is responsible for calling the work or poll methods of the io_service.
   */
  boost::asio::io_service& get_io_service(bool startWorkerThread = true);

  typedef boost::shared_ptr<boost::asio::deadline_timer> AsyncCallHandler;
  /** Call \b callback() in \b usDelay microseconds.
   *  If the method cancel() of the returned timer object is invoked before
   *  expiration of the delay, the call is aborted and callback() is not
   *  called.
   */
  AsyncCallHandler
  asyncCall(boost::function0<void> callback, utime_t usDelay);
}

# include "libport/asio.hxx"

#endif
