/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

// Network API based on boost::asio
#ifndef LIBPORT_ASIO_HH
# define LIBPORT_ASIO_HH

# include <libport/config.h>
# include <libport/sys/socket.h>
# include <libport/iostream>

# include <libport/system-warning-push.hh>

// At least on OS X, many symbols are not available where we expect
// them.  And it is clearly connected to our using
// -fvisibility=hidden.  I (AD) could not exactly pinpoint the
// problem, but at least I know one cure: do not play dirty visibility
// tricks with Asio.
# if defined __GNUC__ && defined __APPLE__
#  pragma GCC visibility push(default)
# endif
# include <boost/asio.hpp>
# if defined LIBPORT_ENABLE_SSL && defined LIBPORT_NO_SSL
#  undef LIBPORT_ENABLE_SSL
# endif
# if defined LIBPORT_ENABLE_SSL
#   include <boost/asio/ssl.hpp>
# endif
# if defined __GNUC__ && defined __APPLE__
#  pragma GCC visibility pop
# endif

# include <boost/function.hpp>

# include <libport/system-warning-pop.hh>

# include <libport/destructible.hh>
# include <libport/export.hh>
# include <libport/finally.hh>
# include <libport/unistd.h>

# include <boost/version.hpp>

namespace libport
{
#if defined WIN32
  typedef boost::asio::detail::socket_type native_handle_type;
  static const native_handle_type invalid_handle =
    (native_handle_type)INVALID_HANDLE_VALUE;
#else
  typedef int native_handle_type;
  static const native_handle_type invalid_handle = -1;
#endif

  class LIBPORT_API AsioDestructible: public Destructible
  {
  protected:
    virtual void doDestroy();
  };

  //FIXME: extend to provide a way to ensure workerThread not started.
  /** Get the io_service handling all asynchronous operations.
   *
   * \param startWorkerThread create a thread in which to run the io_service.
   * If \b startWorkerThread is false on \b first invocation, the user
   * is responsible for calling the work or poll methods of the io_service.
   */
  LIBPORT_API boost::asio::io_service&
  get_io_service(bool startWorkerThread = true);

  /// Get the handle associated to io_service polling thread.
  LIBPORT_API pthread_t get_io_service_poll_thread();
  /** BaseSocket class.
   *
   * This class has a callback-based API: onReadFunc() and onErrorFunc().
   */
  class LIBPORT_API BaseSocket: public AsioDestructible
  {
  public:
    virtual ~BaseSocket(){}
    libport::Finally deletor;
    /// Write data asynchronously to the socket.
    virtual void write(const void* data, size_t length) = 0;
    /// Alias on write() for API compatibility.
    void send(const void* addr, size_t len)
    {
      write(addr, len);
    }
    /// Alias on close() for API compatibility.
    void disconnect()
    {
      close();
    }
    /// Return if the socket is connected to a remote host.
    virtual bool isConnected() const = 0;
    /// Disconnect the socket from the remote host, calls onError.
    virtual void close() = 0;
    /// Get port of remote endpoint
    virtual unsigned short getRemotePort() const = 0;
    /// Get IP address of remote endpoint
    virtual std::string getRemoteHost() const = 0;
    /// Get port of local endpoint
    virtual unsigned short getLocalPort() const = 0;
    /// Get IP address of local endpoint
    virtual std::string getLocalHost() const = 0;
    /// Write synchronously the whole buffer.
    virtual void syncWrite(const void* data, size_t length) = 0;
    /// Synchronously read and return exactly 'length' bytes.
    virtual std::string read(size_t length) = 0;
    // For internal use: start asynchronous reading task
    virtual void startReader() = 0;
#if ! defined WIN32
    virtual native_handle_type stealFD() = 0;
#endif
    virtual native_handle_type getFD() = 0;
    virtual unsigned long bytesReceived() const = 0;
    virtual unsigned long bytesSent() const = 0;
    /// Callback function called each time new data is available.
    boost::function1<bool, boost::asio::streambuf&> onReadFunc;
    /// Callback function called in case of error on the socket.
    boost::function1<void, boost::system::error_code> onErrorFunc;
    /// Mutex to protect access to the above callbacks.
    Lockable callbackLock;
  };

  /// Endpoint on an UDP socket.
  class LIBPORT_API UDPLink
  {
  public:
    virtual ~UDPLink()
    {}
    virtual void reply(const void* data, size_t length) = 0;
    void reply(const std::string& s)
    {
      reply(s.c_str(), s.length());
    }
  };

  #define CHECK do{ \
    if (!base_) throw std::runtime_error("Transport endpoint not connected");\
  }\
    while(0)
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
  class LIBPORT_API Socket: public AsioDestructible
  {
  public:
    Socket(boost::asio::io_service& io = libport::get_io_service())
      : base_(0)
      , io_(io)
    {}
    virtual ~Socket();
    /* Set underlying BaseSocket object, setup its callbacks to call
     * our virtual functions.
     */
    virtual void setBase(BaseSocket*);

    /** Called each time new data is received.
     *   \return the number of bytes used in buffer. The remaining data will
     *   be passed again to this function as soon as at least an extra byte
     *   is available.
     */
    virtual size_t onRead(const void*, size_t length)
    {
      return length;
    }

    /** Called in case of error on the socket. By default, do nothing.
     */
    virtual void onError(boost::system::error_code);

    /** Called on socket connection
     */
    virtual void onConnect()
    {}

    /// Ask for the asynchronous destruction of this object.
    virtual void destroy();

    /// Asynchronous write
    void write(const void* data, size_t length)
    {
      CHECK;
      base_->write(data, length);
    }
    /// Alias on write() for API compatibility.
    void send(const void* addr, size_t len)
    {
      write(addr, len);
    }
    void send(const std::string& s)
    {
      write(s.c_str(), s.length());
    }

    void syncWrite(const void* data, size_t length);
    void syncWrite(const std::string& s);

    /// Synchronously read and return exactly 'length' bytes.
    std::string read(size_t length)
    {
      CHECK;
      if (base_)
        return base_->read(length);
      else
        throw std::runtime_error("Socket not initialized");
    }

    void close()
    {
      if (base_)
        base_->close();
    }

    unsigned short getRemotePort() const { CHECK;return base_->getRemotePort();}
    std::string getRemoteHost() const    { CHECK;return base_->getRemoteHost();}
    unsigned short getLocalPort() const  { CHECK;return base_->getLocalPort();}
    std::string getLocalHost() const     { CHECK;return base_->getLocalHost();}
    unsigned long bytesReceived() const  { CHECK;return base_->bytesReceived();}
    unsigned long bytesSent() const      { CHECK;return base_->bytesSent();}
    bool isConnected() const      {return base_ ? base_->isConnected() : false;}

    /** Connect to a remote host.
     * \param host hostname to connect to.
     * \param port port to connect to, as a service name or an int.
     * \param udp  whether UDP instead of TCP.
     * \param usTimeout timeout in microseconds, 0 meaning none.
     * \param asynchronous make an asynchronous attempt. The function will
     *    return immediately, and either onConnect will be called when the
     *    connection succeeds, or onError() will be called.
     * \return an error code if the connection failed.
     *
     */
    boost::system::error_code
    connect(const std::string& host, const std::string& port,
            bool udp = false, useconds_t usTimeout = 0,
            bool asynchronous = false);

    boost::system::error_code
    connect(const std::string& host, unsigned port,
            bool udp = false, useconds_t usTimeout = 0,
            bool asynchronous = false);

# if 103600 <= BOOST_VERSION

    /** Open a serial port.
     * \param the device to use (ex: ttyUSB0,ttyS0 under linux,
     *    COM1 under windows.
     * \param rate bit-rate (9600, 19200, 38400, 57600, 115200...)
     * \param async_reader start asynchronous reading task if true. Setting
     *    this to false meanse your onRead() function will never be called,
     *    you will have to use synchronous calls to read().
     */
    boost::system::error_code
    open_serial(const std::string& device, unsigned int rate,
                bool async_reader = true);

# endif

    /** Make an asynchronous connection attempt to a remote host.
     *  This API garantees that either the onConnect() or the onError() function
     *  will be called.
     * \param host hostname to connect to.
     * \param port port to connect to, as a service name or an int.
     * \param usTimeout timeout in microseconds, 0 meaning none.
     */
    void async_connect(const std::string& host, const std::string& port,
                       bool udp=false, useconds_t usTimeout = 0)
    {
      connect(host, port, udp, usTimeout, true);
    }


    typedef boost::function0<Socket*> SocketFactory;
    /** Listen using udp.
     * Call onRead(data, length, link) for each new packet. Link can be used
     * to reply to the sender through its UDPLink::reply() method.
     * @return the local port that was bound.
     */
    static unsigned short listenUDP(const std::string& host,
                                    const std::string& port,
                            boost::function3<void, const void*, size_t,
                            boost::shared_ptr<UDPLink> > onRead,
                            boost::system::error_code& erc,
                            boost::asio::io_service& s
                              = libport::get_io_service());

    /// Close UDP socket listening on \b port.
    static bool closeUDP(unsigned short port);
    /** Listen using TCP or UDP.
     *
     * \param f a socket factory. For each new connection the function will
     * be called and the resulting socket object bound to the new connection.
     */
    boost::system::error_code
    listen(SocketFactory f, const std::string& host,
           const std::string& port, bool udp = false);

    boost::system::error_code
    listen(SocketFactory f, const std::string& host,
           unsigned port, bool udp = false);

#if defined LIBPORT_ENABLE_SSL

    boost::system::error_code
    connectSSL(const std::string& host, const std::string& port,
               useconds_t usTimeout = 0,
               bool asynchronous = false,
               boost::asio::ssl::context_base::method ctx
                = boost::asio::ssl::context::sslv23_client,
               boost::asio::ssl::context::options options
                = boost::asio::ssl::context::verify_none,
               const std::string& privateKeyFile = "",
               const std::string& certChainFile = "",
               const std::string& tmpDHFile = "" ,
               const std::string& cipherList = ""
               );
    boost::system::error_code
    listenSSL(SocketFactory f,
              const std::string& host,
              const std::string& port,
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
    native_handle_type stealFD();
    /// Get file descriptor from Socket.
    native_handle_type getFD();
    /// Set file descriptor
    template<class Sock>
    void setFD(native_handle_type fd, typename Sock::protocol_type proto);
    /** Sleep for specified amount of time, polling if current thread is
     * the asio worker thread
     */
    static void sleep(useconds_t duration);
    boost::asio::io_service& get_io_service();
  protected:
    virtual void doDestroy();
    bool onRead_(boost::asio::streambuf&);
    std::string buffer;
    BaseSocket* base_;
  private:
    template<typename Proto, typename BaseFactory> boost::system::error_code
    listenProto(SocketFactory f, const std::string& host,
                const std::string&port, BaseFactory bf);
    template<typename Proto, typename BaseFactory> boost::system::error_code
    connectProto(const std::string& host, const std::string& port,
                 useconds_t timeout, bool async, BaseFactory bf);
    boost::asio::io_service& io_;
  };
#undef CHECK
  /** Wrapper of libport::Socket to be able to use Socket without inherit from
   *  it.
   */
  class LIBPORT_API ConcreteSocket: public Socket
  {
  public:
    typedef boost::function2<size_t, const void*, size_t> onread_type;
    typedef boost::function1<void, boost::system::error_code> onerror_type;
    typedef boost::function0<void> onconnect_type;
    typedef Socket super_type;

    ConcreteSocket(boost::asio::io_service& io = libport::get_io_service())
      : Socket(io)
      , onconnect_(0)
      , onerror_(0)
      , onread_(0)
    {}

    ConcreteSocket& onConnect(onconnect_type cb)
    {
      onconnect_ = cb;
      return *this;
    }

    ConcreteSocket& onError(onerror_type cb)
    {
      onerror_ = cb;
      return *this;
    }

    ConcreteSocket& onRead(onread_type cb)
    {
      onread_ = cb;
      return *this;
    }

    virtual void onConnect()
    {
      if (onconnect_)
        onconnect_();
    }
    virtual void onError(boost::system::error_code erc)
    {
      if (onerror_)
        onerror_(erc);
    }
    virtual size_t onRead(const void* data, size_t length)
    {
      if (onread_)
        return onread_(data, length);
      else
        return length;
    }

  private:
    onconnect_type onconnect_;
    onerror_type onerror_;
    onread_type onread_;
  };

  LIBPORT_API bool
  isPollThread();

  /** Poll on an io_service for given duration in microseconds.
   *  @param duration max duration for which to poll in microseconds
   *  @param once process at most one handler before returning if true
   *  @return the number of handler functions that were called.
   */
  LIBPORT_API size_t
  pollFor(useconds_t duration, bool once = false,
          boost::asio::io_service& io = get_io_service());

  typedef boost::shared_ptr<boost::asio::deadline_timer> AsyncCallHandler;
  /** Call \b callback() in \b usDelay microseconds.
   *  If the method cancel() of the returned timer object is invoked before
   *  expiration of the delay, the call is aborted and callback() is not
   *  called.
   */
  LIBPORT_API AsyncCallHandler
  asyncCall(boost::function0<void> callback, useconds_t usDelay,
            boost::asio::io_service& io = get_io_service());

  /** Return a pair of connected sockets. First is read-only, second write-only.
   */
  LIBPORT_API void
  makePipe(std::pair<Socket*, Socket*>,
           boost::asio::io_service& io = get_io_service());
}

# include "libport/asio.hxx"

#endif
