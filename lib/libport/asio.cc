/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

// This compilation unit must not have any ssl-dependent code.
// Said code goes in asio-ssl.cc
#define LIBPORT_NO_SSL

#include <libport/asio.hh>
#include <libport/containers.hh>
#include <libport/detect-win32.h>
#include <libport/format.hh>
#include <libport/thread.hh>

#include "asio-impl.hxx"

namespace libport
{

  namespace netdetail
  {
    // Wrap a boost::asio stream adding Socket interface
    template<typename T> class SocketWrapper: public T
    {
    public:
      typedef SocketWrapper<T> lowest_layer_type;
      struct addr
      {
        std::string to_string()
        {
          return std::string();
        }
      };

      struct endpoint
      {
        std::string host()
        {
          return std::string();
        };

        unsigned short port()
        {
          return 0;
        };

        addr address()
        {
          return addr();
        };

        int protocol()
        {
          return 0;
        }

      };
      SocketWrapper(boost::asio::io_service& s)
        : T(s)
      {}
#ifdef WIN32
      SocketWrapper(boost::asio::io_service& s, HANDLE f)
        : T(s, f)
      {}
#else
      SocketWrapper(boost::asio::io_service& s, int f)
        : T(s, f)
      {}
#endif
      void shutdown(int, boost::system::error_code&)
      {
        boost::system::error_code erc;
        if (T::is_open())
          T::close(erc);
      }
      endpoint remote_endpoint() { return endpoint();}
      endpoint local_endpoint() { return endpoint();}
      SocketWrapper<T>& lowest_layer() {return *this;}
      void assign(int, int);
      static const int shutdown_both = 0;
    };
    class UDPLinkImpl: public UDPLink
    {
    public:
      UDPLinkImpl(boost::asio::ip::udp::socket& socket,
                  boost::asio::ip::udp::endpoint endpoint,
                  Destructible::DestructionLock destructionLock)
        : socket_(socket)
        , endpoint_(endpoint)
        , destructionLock_(destructionLock)
      {}
      virtual void reply(const void* data, size_t length);
    private:
      boost::asio::ip::udp::socket& socket_;
      boost::asio::ip::udp::endpoint endpoint_;
      Destructible::DestructionLock destructionLock_;
    };

    class UDPSocket: public Destructible
    {
    public:
      UDPSocket(boost::asio::io_service& io = get_io_service());
      typedef boost::function3<void, const void*, size_t,
                               boost::shared_ptr<UDPLink> > onread_type;
      onread_type onRead;
      void start_receive();
      void handle_receive(const boost::system::error_code& error, size_t sz);
      unsigned short getLocalPort();
    private:
      std::vector<char> recv_buffer_;
      static const int default_size_ = 65535;
      boost::asio::ip::udp::endpoint remote_endpoint_;
      boost::asio::ip::udp::socket socket_;
      friend class libport::Socket;
    };

    UDPSocket::UDPSocket(boost::asio::io_service& io)
      :socket_(io)
    {
      recv_buffer_.resize(default_size_);
    }

    void
    UDPSocket::start_receive()
    {
      socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_),
        remote_endpoint_,
        boost::bind(&UDPSocket::handle_receive, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    }

    void
    UDPSocket::handle_receive(const boost::system::error_code& err,
                              size_t sz)
    {
      if (err)
        return;
      boost::shared_ptr<UDPLink> l(new UDPLinkImpl(socket_, remote_endpoint_,
                                                   getDestructionLock()));
      onRead(&recv_buffer_[0], sz, l);
      start_receive();
    }

    unsigned short UDPSocket::getLocalPort()
    {
      return socket_.local_endpoint().port();
    }

    void
    UDPLinkImpl::reply(const void* data, size_t length)
    {
      socket_.send_to(boost::asio::buffer(data, length), endpoint_);
    }


    void
    delete_check(boost::system::error_code erc,
                 SocketImpl<udpsock>*s,
                 Destructible::DestructionLock,
                 std::string* buf)
    {
      if (erc)
      {
        std::cerr <<"Socket error: " << erc.message() << std::endl;
        BlockLock bl(s->callbackLock);
        if (s->onErrorFunc)
          s->onErrorFunc(erc);
      }
      delete buf;
    }

    template<>
    inline void
    send_bounce(SocketImpl<udpsock>* s, const void* buffer, size_t length)
    {
      std::string* buf = new std::string(static_cast<const char*>(buffer),
                                         length);
      s->base_->async_send(
        boost::asio::buffer(*buf),
        boost::bind(&delete_check, _1, s, s->getDestructionLock(), buf));
    }



    void
    recv_bounce(SocketImpl<udpsock>*s,
                SocketImpl<udpsock>::DestructionLock lock,
                boost::system::error_code erc, size_t recv)
    {
      std::ostream stream(&s->readBuffer_);
      stream.write(&s->udpBuffer_[0], std::streamsize(recv));
      s->onReadDemux(lock, erc, recv);
    }

    template<>
    inline void
    read_or_recv(SocketImpl<udpsock>*s,
                 SocketImpl<udpsock>::DestructionLock lock)
    {
      if (!s->udpBuffer_.size())
        s->udpBuffer_.resize(65535);
      s->base_->async_receive(boost::asio::buffer(s->udpBuffer_),
                              boost::bind(&recv_bounce, s, lock, _1, _2));
    }

    void
    onConnect(boost::system::error_code erc,
              boost::asio::deadline_timer & timer,
              libport::Semaphore& sem,
              boost::system::error_code& caller_erc)
    {
      caller_erc = erc;
      // If timer was already canceled, cancel is a noop and does no harm.
      timer.cancel();
      sem++;
    }
  }

  boost::asio::io_service&
  Socket::get_io_service()
  {
    return io_;
  }



  boost::system::error_code
  Socket::connect(const std::string& host,
                  const std::string& port,
                  bool udp,
                  useconds_t timeout,
                  bool async)
  {
    if (udp)
      return connectProto<boost::asio::ip::udp>(
        host, port, timeout, async,
        &netdetail::SocketImpl<boost::asio::ip::udp::socket>::create);
    else
      return connectProto<boost::asio::ip::tcp>(
        host, port, timeout, async,
        &netdetail::SocketImpl<boost::asio::ip::tcp::socket>::create);
  }

  boost::system::error_code
  Socket::connect(const std::string& host,
                  unsigned port,
                  bool udp,
                  useconds_t timeout,
                  bool async)
  {
    return connect(host, string_cast(port), udp, timeout, async);
  }


  boost::system::error_code
  Socket::listen(SocketFactory f,
                 const std::string& host,
                 const std::string& port,
                 bool udp)
  {
    LIBPORT_USE(udp);
    aver(!udp);
    boost::system::error_code erc;
    erc = Socket::listenProto<boost::asio::ip::tcp>(
      f, host, port,
      &netdetail::SocketImpl<boost::asio::ip::tcp::socket>::create);
    return erc;
  }

  boost::system::error_code
  Socket::listen(SocketFactory f,
                 const std::string& host,
                 unsigned port,
                 bool udp)
  {
    return listen(f, host, string_cast(port), udp);
  }

  void
  AsioDestructible::doDestroy()
  {
    get_io_service().post(boost::bind(netdetail::deletor<AsioDestructible>,
                                      this));
  }

  void
  Socket::doDestroy()
  {
    io_.post(boost::bind(netdetail::deletor<AsioDestructible>,
                                      this));
  }

  void
  Socket::onError(boost::system::error_code)
  {
    // Nothing
  }

  namespace netdetail
  {

    void
    timer_trigger(boost::shared_ptr<boost::asio::deadline_timer>,
                  boost::function0<void> callback,
                  boost::system::error_code erc)
    {
      if (!erc)
        callback();
    }

  }

  AsyncCallHandler
  asyncCall(boost::function0<void> callback, useconds_t usDelay,
            boost::asio::io_service& io)
  {
    AsyncCallHandler
      res(new boost::asio::deadline_timer(io));
    res->expires_from_now(boost::posix_time::microseconds(usDelay));
    res->async_wait(boost::bind(&netdetail::timer_trigger, res, callback, _1));
    return res;
  }
  void
  runIoService(boost::asio::io_service* io)
  {
    while (true)
    {
      // Used so that io->run() never returns.
      boost::asio::io_service::work work(*io);
      io->run();
      std::cerr << "The impossible happened" << std::endl;
      io->reset();
    }
  }

  static pthread_t asio_worker_thread;

  pthread_t get_io_service_poll_thread()
  {
    return asio_worker_thread;
  }

  boost::asio::io_service&
  get_io_service(bool startWorkerThread)
  {
    static boost::asio::io_service* io = 0;
    static bool hasWorkerThread = false;
    if (!startWorkerThread && hasWorkerThread)
    {
      std::cerr <<"fatal, worker thread already running" << std::endl;
      abort();
    }
    if (!io)
    {
      io = new boost::asio::io_service;
      if (startWorkerThread)
      {
        hasWorkerThread = true;
        asio_worker_thread =
          startThread(boost::bind(&runIoService, io));
      }
      else
        asio_worker_thread = pthread_self();
    }
    return *io;
  }

#if ! defined WIN32
  int
  Socket::stealFD()
  {
    if (base_)
      return base_->stealFD();
    else
      return invalid_handle;
  }
#endif

  native_handle_type
  Socket::getFD()
  {
    if (base_)
      return base_->getFD();
    else
      return invalid_handle;
  }

  void
  Socket::destroy()
  {
    // Lock in case the user calls destroy() in its error handler.
    Destructible::DestructionLock l = getDestructionLock();
    // It is safe to reach that point with an open socket.
    close();
    AsioDestructible::destroy();
  }

  Socket::~Socket()
  {
    /* FIXME: ensure no false positive before activating
       if (!checkDestructionPermission())
       std::cerr <<"WARNING, attempting to delete a Socket that is still in use."
       << "\n\tThis is likely a bug if you overrided onRead or onError"
       << "\n\tSee Socket documentation for more informations."
       << std::endl;
    */
    wasDestroyed();
    if (base_)
    {
      Destructible::DestructionLock l = base_->getDestructionLock();
      base_->close();
      base_->destroy();
      BlockLock bl(base_->callbackLock);
      base_->onReadFunc = 0;
      base_->onErrorFunc = 0;
      base_ = 0;
    }
    // FIXME: optimize
    while (!checkDestructionPermission())
    {
      sleep(100);
    }
    //waitForDestructionPermission();
  }
  static std::map<unsigned short,  netdetail::UDPSocket*> udp_map;
  bool Socket::closeUDP(unsigned short port)
  {
    netdetail::UDPSocket* s = libport::find0(udp_map, port);
    if (s)
    {
      delete s;
      udp_map.erase(port);
      return true;
    }
    else
      return false;
  }
  unsigned short
  Socket::listenUDP(const std::string& host,
                    const std::string& port,
                    netdetail::UDPSocket::onread_type onRead,
                    boost::system::error_code& erc,
                    boost::asio::io_service& io)
  {
    using namespace boost::asio::ip;
    netdetail::UDPSocket* s = new netdetail::UDPSocket(io);
    s->onRead = onRead;
    /* On some configurations, the resolver will resolve an ipv6 address even
     * if this protocol is not supported by the system. So try to bind using all
     * the endopints until one succeeds, and not just the first. */
    udp::resolver::query query(host, port);
    udp::resolver resolver(io);
    udp::resolver::iterator iter = resolver.resolve(query, erc);
    if (erc)
      return 0;
    // Careful to use the protocol reported by the endpoint.
    while (iter != udp::resolver::iterator())
    {
      s->socket_.open(iter->endpoint().protocol(), erc);
      if (!erc)
        s->socket_.bind(iter->endpoint(), erc);
      if (!erc)
        goto ok;
      iter++;
    }
    if (!erc)
      erc =
        netdetail::errorcodes::make_error_code(
          netdetail::errorcodes::bad_address);
    return 0;
  ok:
    s->start_receive();
    unsigned short lp = s->getLocalPort();
    udp_map[lp] = s;
    return lp;
  }

  void
  Socket::setBase(BaseSocket* b)
  {
    base_ = b;
    BlockLock bl(base_->callbackLock);
    base_-> onReadFunc = boost::bind(&Socket::onRead_, this, _1);
    base_->onErrorFunc = boost::bind(&Socket::onError, this, _1);
  }

  bool
  Socket::onRead_(boost::asio::streambuf& buf)
  {
    DestructionLock lock = getDestructionLock();
    // Dump the stream in our linear buffer
    std::istream is(&buf);
    static const int blockSize = 1024;
    while (is.good())
    {
      size_t oldBufferLength = buffer.length();
      buffer.resize(oldBufferLength + blockSize);
      is.read(&buffer[0] + oldBufferLength, blockSize);
      long len = is.gcount();
      buffer.resize(oldBufferLength + len);
    }
    // Call onRead until it eats 0 characters.
    size_t r;
    do
    {
      r = onRead(buffer.c_str(), buffer.length());
      if (r)
        buffer = buffer.substr(r, buffer.npos);
    }
    while(r && !buffer.empty());
    return true;
  }

  void
  Socket::sleep(useconds_t duration)
  {
    //FIXME: implement for real
    if (isPollThread())
      pollFor(duration);
    else
      usleep(duration);
  }

  static void stop_io_service(boost::asio::io_service& io,
                              bool& timer_called)
  {
    timer_called = true;
    io.stop();
  }
  size_t pollFor(useconds_t duration, bool once, boost::asio::io_service& io)
  {
    boost::asio::io_service::work work(io);
    io.reset();
    bool timer_called = false;
    AsyncCallHandler asc =
      asyncCall(boost::bind(&stop_io_service, boost::ref(io),
                            boost::ref(timer_called)), duration, io);
    size_t res = 0;
    if (once)
      res = io.run_one();
    else
      res = io.run();
    if (timer_called)
      res--;
    asc->cancel();
    return res;
  }

  bool isPollThread()
  {
    return pthread_self() == asio_worker_thread;
  }

# if 103600 <= BOOST_VERSION

  boost::system::error_code
  Socket::open_serial(const std::string& device,
                      unsigned int rate,
                      bool async_reader)
  {
    boost::system::error_code erc;
    typedef netdetail::SocketWrapper<boost::asio::serial_port>
      SerialPortWrapper;
    SerialPortWrapper& sp = *new SerialPortWrapper(get_io_service());
    sp.open(device, erc);
    if (erc)
      return erc;
    sp.set_option(boost::asio::serial_port::baud_rate(rate), erc);
    // 8N1 mode, might not be the default so force it.
    sp.set_option(boost::asio::serial_port::character_size(8), erc);
    sp.set_option(boost::asio::serial_port::parity(
      boost::asio::serial_port::parity::none), erc);
    sp.set_option(boost::asio::serial_port::stop_bits(
      boost::asio::serial_port::stop_bits::one), erc);
    sp.set_option(boost::asio::serial_port::flow_control(
      boost::asio::serial_port::flow_control::none), erc);
    if (erc)
      return erc;
    BaseSocket* sb = netdetail::SocketImpl<SerialPortWrapper>::create(&sp);
    setBase(sb);
    if (async_reader)
      sb->startReader();
    onConnect();

    return erc;
  }

# endif

  void
  makePipe(std::pair<Socket*, Socket*> s, boost::asio::io_service& io)
  {
    using namespace boost::asio;
    using namespace netdetail;
#ifdef WIN32
    // Implement using named pipes.  The format name is specified:
    // http://msdn.microsoft.com/en-us/library/aa365150(VS.85).aspx
    std::string name =
      libport::format("\\\\.\\pipe\\libport-%s-%s-%s",
                      getpid(), utime(), rand());

    // Do not forget the OVERLAPPED flag as boost::asio does...asio.
    HANDLE h1 =
      CreateNamedPipe(name.c_str(),
        PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
        2, 512, 512, 0, NULL);
    if (h1 == INVALID_HANDLE_VALUE)
      throw std::runtime_error
        (libport::format("CreateNamedPipe(%s): %s",
                         name, strerror(0)));
    BaseSocket* b1 =
      SocketImpl<SocketWrapper<windows::stream_handle> >::create
      (new SocketWrapper<windows::stream_handle>(io, h1));

    // Calling CreatePipe should work for the second end but it fails with
    // 'access denied'.
    HANDLE h2 = CreateFile(name.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE,
                           NULL, OPEN_EXISTING,
                           SECURITY_ANONYMOUS | FILE_FLAG_OVERLAPPED,
                           NULL);
   if (h2 == INVALID_HANDLE_VALUE)
         throw std::runtime_error
           (libport::format("CreateFile(%s): %s",
                            name, strerror(0)));
    BaseSocket* b2 =
          SocketImpl<SocketWrapper<windows::stream_handle> >::create
          (new SocketWrapper<windows::stream_handle>(io, h2));
#else
    // implement using POSIX pipe()
    int fd[2];
    if (pipe(fd) == -1)
      throw std::runtime_error(std::string("pipe:") + strerror(errno));
    BaseSocket* b1 =
      SocketImpl<SocketWrapper<posix::stream_descriptor> >::create(
       new SocketWrapper<posix::stream_descriptor>(io, fd[0]));
    BaseSocket* b2 =
      SocketImpl<SocketWrapper<posix::stream_descriptor> >::create(
      new SocketWrapper<posix::stream_descriptor>(io, fd[1]));
#endif
    s.first->setBase(b1);
    s.second->setBase(b2);
    b1->startReader();
    //No reader at b2 end.
    s.first->onConnect();
    s.second->onConnect();
  }
}
