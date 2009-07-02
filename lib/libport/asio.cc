#include <libport/asio.hh>


namespace libport
{
  static pthread_t asio_worker_thread;
  boost::asio::io_service&
  get_io_service(bool startWorkerThread)
  {
    static boost::asio::io_service* io = 0;
    static bool hasWorkerThread = false;
    if (!startWorkerThread && hasWorkerThread)
    {
      std::cerr <<"fatal, worker thread allready running" << std::endl;
      abort();
    }
    if (!io)
    {
      io = new boost::asio::io_service;
      if (startWorkerThread)
      {
        hasWorkerThread = true;
        asio_worker_thread =
          libport::startThread(boost::bind(&netdetail::runIoService, io));
      }
      else
        asio_worker_thread = pthread_self();
    }
    return *io;
  }

  int
  Socket::stealFD()
  {
    if (base_)
      return base_->stealFD();
    else
      return -1;
  }

  int
  Socket::getFD()
  {
    if (base_)
      return base_->getFD();
    else
      return -1;
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
    // FIXME: this line asserts that another thread is running io_services.
    waitForDestructionPermission();
  }

  Socket::Handle
  Socket::listenUDP(const std::string& host,
                    const std::string& port,
                    netdetail::UDPSocket::onread_type onRead,
                    boost::system::error_code& erc)
  {
    using namespace boost::asio::ip;
    netdetail::UDPSocket* s = new netdetail::UDPSocket();
    s->onRead = onRead;
    /* On some configurations, the resolver will resolve an ipv6 address even
     * if this protocol is not supported by the system. So try to bind using all
     * the endopints until one succeeds, and not just the first. */
    udp::resolver::query query(host, port);
    udp::resolver resolver(get_io_service());
    udp::resolver::iterator iter = resolver.resolve(query, erc);
    if (erc)
      return Handle();
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
    return Handle();
    ok:
    s->start_receive();
    return Handle();
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
    int r;
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
  Socket::sleep(utime_t duration)
  {
    //FIXME: implement for real
    if (isPollThread())
      pollFor(duration, get_io_service());
    else
      usleep(duration);
  }

  static void stop_io_service(boost::asio::io_service& io)
  {
    io.stop();
  }
  void pollFor(utime_t duration, boost::asio::io_service& io)
  {
    boost::asio::io_service::work work(io);
    AsyncCallHandler asc =
      asyncCall(boost::bind(&stop_io_service, boost::ref(io)), duration);
    io.reset();
    io.run();
  }

  bool isPollThread()
  {
    return pthread_self() == asio_worker_thread;
  }

# if BOOST_VERSION >= 103600

  boost::system::error_code
  Socket::open_serial(const std::string& device,
                      unsigned int rate)
  {
    boost::system::error_code erc;

    Rs232 sp(libport::get_io_service());
    sp.open(device, erc);
    sp.set_option(boost::asio::serial_port::baud_rate(rate), erc);
    typedef libport::netdetail::SocketImpl<Rs232> SerBase;
    SerBase* sb = (SerBase*)SerBase::create(&sp);
    this->setBase(sb);
    sb->startReader();

    return erc;
  }

# endif
}
