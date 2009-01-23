#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/construct.hpp>

// #define LIBPORT_ECHO(S) std::cerr << __FILE__ ":" <<  __LINE__ << ": " #S ": " << S << std::endl

#include <libport/thread.hh>
#include <libport/lockable.hh>
#include <libport/semaphore.hh>

namespace libport {
namespace netdetail {

  typedef ::libport::Socket::SocketFactory SocketFactory;

  typedef boost::asio::basic_datagram_socket<boost::asio::ip::udp,
  boost::asio::datagram_socket_service<boost::asio::ip::udp> > udpsock;

  class SocketImplBase: public BaseSocket, protected libport::Lockable
  {
    public:
      SocketImplBase(): current_(-1), pending_(false) {}
      /// Start an asynchronous read operation.
      virtual void startReader() = 0;
    protected:
      /// Write double-buffer.
      boost::asio::streambuf buffers_[2];
      /// Read buffer.
      boost::asio::streambuf readBuffer_;
      /// Buffer id currently engaged in an async write, -1 for none
      int current_;
      /// Second buffer has data to write.
      bool pending_;
      friend class libport::Socket;
      template<class Stream> friend class SocketImpl;
  };
  template<class Stream> class SocketImpl;
  template<typename Stream, typename Lock> void
  read_or_recv(SocketImpl<Stream>* s,
               Lock lock);
  template<class Stream> class SocketImpl
    : public SocketImplBase
  {
    public:
      SocketImpl():base_(0) {}
      ~SocketImpl() { delete base_;}
      void write(const void* data, unsigned int length);
      void close();
      unsigned short getRemotePort();
      std::string getRemoteHost();
      unsigned short getLocalPort();
      std::string getLocalHost();
      bool isConnected();
      template<typename Acceptor, typename BaseFactory> static void
        onAccept(boost::system::error_code erc, Stream* s, SocketFactory fact,
               Acceptor *a, BaseFactory bf);
      template<typename Acceptor, typename BaseFactory> static void
        acceptOne(SocketFactory fact, Acceptor *a, BaseFactory bf);
      static BaseSocket* create(Stream* base);
      void startReader();
    private:
      Stream* base_;
      void continueWrite(DestructionLock lock, boost::system::error_code erc,
                         size_t sz);
      void onReadDemux(DestructionLock lock, boost::system::error_code erc);

      friend class libport::Socket;
      template<class T, class L> friend void read_or_recv(SocketImpl<T>*, L);
      template<class T> friend void send_bounce(SocketImpl<T>*,const void*,int);
      friend void
      recv_bounce(SocketImpl<udpsock>*s, AsioDestructible::DestructionLock lock,
                  boost::system::error_code erc, size_t recv);
      std::vector<char> udpBuffer_;
    };

    // Acceptor implementation
    #define ACCEPTOR_FAIL \
      {throw std::runtime_error("Call not implemented for Acceptors");}
    template<class Acceptor> class AcceptorImpl
    :public BaseSocket
    {
      public:
      AcceptorImpl(Acceptor* base);
      ~AcceptorImpl();
      void write(const void*, unsigned int) ACCEPTOR_FAIL
      bool isConnected() {return false;}
      void close();
      unsigned short getRemotePort() ACCEPTOR_FAIL
      std::string getRemoteHost() ACCEPTOR_FAIL
      unsigned short getLocalPort();
      std::string getLocalHost();
      private:
      Acceptor* base_;
    };
    #undef ACCEPTOR_FAIL

    template<class Acceptor>
    AcceptorImpl<Acceptor>::AcceptorImpl(Acceptor* base)
      : base_(base)
    {
    }

    template<class Acceptor>
    AcceptorImpl<Acceptor>::~AcceptorImpl()
    {
      close();
      delete base_;
    }

    template<class Acceptor>
    void
    AcceptorImpl<Acceptor>::close()
    {
      boost::system::error_code erc;
      if (base_)
        base_->close(erc);
      base_ = 0;
    }
    template<class Acceptor> unsigned short
    AcceptorImpl<Acceptor>::getLocalPort()
    {
      return base_->local_endpoint().port();
    }
    template<class Acceptor> std::string
    AcceptorImpl<Acceptor>::getLocalHost()
    {
      return base_->local_endpoint().address().to_string();
    }
    inline void
    runIoService(boost::asio::io_service* io)
    {
      // Trick to have io->run() never return.
      boost::asio::io_service::work work(*io);
      io->run();
    }

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
      virtual void reply(const void* data, int length);
      private:
      boost::asio::ip::udp::socket& socket_;
      boost::asio::ip::udp::endpoint endpoint_;
      Destructible::DestructionLock destructionLock_;
    };

    class UDPSocket: public Destructible
    {
      public:
      UDPSocket();
      typedef boost::function3<void, const void*, int,
                               boost::shared_ptr<UDPLink> > onread_type;
      onread_type onRead;
      void start_receive();
      void handle_receive(const boost::system::error_code& error, size_t sz);
      private:
      boost::array<char, 2000> recv_buffer_;
      boost::asio::ip::udp::endpoint remote_endpoint_;
      boost::asio::ip::udp::socket socket_;
      friend class libport::Socket;
    };
    inline
    UDPSocket::UDPSocket()
    :socket_(get_io_service())
    {
    }

    inline void
    UDPSocket::start_receive()
    {
      socket_.async_receive_from(boost::asio::buffer(recv_buffer_),
        remote_endpoint_,
        boost::bind(&UDPSocket::handle_receive, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    }
    inline void
    UDPSocket::handle_receive(const boost::system::error_code&,
                              size_t sz)
    {
      boost::shared_ptr<UDPLink> l(new UDPLinkImpl(socket_, remote_endpoint_,
                                                   getDestructionLock()));
      onRead(&recv_buffer_[0], sz, l);
      start_receive();
    }
#ifndef LIBPORT_NO_SSL
  struct SSLSettings
  {
    boost::asio::ssl::context_base::method context;
    boost::asio::ssl::context::options options;
    std::string privateKeyFile;
    std::string certChainFile;
    std::string tmpDHFile;
    std::string cipherList;
  };
#endif
  inline void
  UDPLinkImpl::reply(const void* data, int length)
  {
    socket_.send_to(boost::asio::buffer(data, length), endpoint_);
  }
  // FIXME: use the delete_ptr in boost::lambda.
  template<class T> void deletor(T* ptr)
  {
    delete ptr;
  }

  template<typename Stream> void
  SocketImpl<Stream>::close()
  {
    Destructible::DestructionLock l = getDestructionLock();
    if (base_->lowest_layer().is_open())
    {
      boost::system::error_code erc;
      base_->lowest_layer().shutdown(Stream::lowest_layer_type::shutdown_both,
                                     erc);
      base_->lowest_layer().close(erc);
    }
  }

  template<typename Stream> unsigned short
  SocketImpl<Stream>::getRemotePort()
  {
    return base_->lowest_layer().remote_endpoint().port();
  }

  template<typename Stream> std::string
  SocketImpl<Stream>::getRemoteHost()
  {
    return base_->lowest_layer().remote_endpoint().address().to_string();
  }

  template<typename Stream> unsigned short
  SocketImpl<Stream>::getLocalPort()
  {
    return base_->lowest_layer().local_endpoint().port();
  }

  template<typename Stream> std::string
  SocketImpl<Stream>::getLocalHost()
  {
    return base_->lowest_layer().local_endpoint().address().to_string();
  }

  template<typename Stream> bool
  SocketImpl<Stream>::isConnected()
  {
    return base_->lowest_layer().is_open();
  }

  template<class T> void send_bounce(SocketImpl<T>* s,const void* buffer,
                                     int length)
  {
    libport::BlockLock bl(s);
    std::ostream stream(&s->buffers_[s->current_==-1 ? 0:1-s->current_]);
    stream.write((const char*)buffer, length);
    s->pending_ = true;
    if (s->current_ == -1)
    {
      s->current_ = 1; // We wrote on 0, continueWrite will swap.
      s->continueWrite(s->getDestructionLock(), boost::system::error_code(), 0);
    }
  }
  inline void
  delete_check(boost::system::error_code erc, std::string* buf)
  {
    if (erc)
      std::cerr <<"Socket error: " << erc.message() << std::endl;
    delete buf;
  }
  template<> inline void
  send_bounce(SocketImpl<udpsock>* s, const void* buffer,
                              int length)
  {
    std::string* buf = new std::string(static_cast<const char*>(buffer),
                                       length);
    s->base_->async_send(boost::asio::buffer(*buf),
                         boost::bind(&delete_check, _1, buf));
  }

  template<typename Stream> void
  SocketImpl<Stream>::write(const void* buffer, unsigned int length)
  {
    send_bounce(this, buffer, length);
  }

  template<typename Stream> void
  SocketImpl<Stream>::continueWrite(DestructionLock lock, boost::system::error_code er, size_t)
  {
    if (er)
    {
      if (onErrorFunc)
	onErrorFunc(er);
      close();
      return;
    }
    libport::BlockLock bl(this);
    current_ = 1 - current_;
    if (pending_)
      boost::asio::async_write(*base_, buffers_[current_],
	  boost::bind(&SocketImpl<Stream>::continueWrite, this, lock,  _1, _2));
    else
    {
      current_ = -1;
    }
    pending_ = false;
  }


  template<typename Stream, typename Lock> void
  read_or_recv(SocketImpl<Stream>* s,
               Lock lock)
  {
    boost::asio::async_read(*s->base_, s->readBuffer_,  boost::asio::transfer_at_least(1),
	    boost::bind(&SocketImpl<Stream>::onReadDemux, s, lock, _1));
  }
  inline void
  recv_bounce(SocketImpl<udpsock>*s, SocketImpl<udpsock>::DestructionLock lock,
              boost::system::error_code erc, size_t recv)
  {
    std::ostream stream(&s->readBuffer_);
    stream.write(&s->udpBuffer_[0], recv);
    s->onReadDemux(lock, erc);
  }
  template<> inline void
  read_or_recv(SocketImpl<udpsock>*s,
               SocketImpl<udpsock>::DestructionLock lock)
  {
    s->udpBuffer_.resize(2000);
    s->base_->async_receive(boost::asio::buffer(&s->udpBuffer_[0], 2000),
	    boost::bind(&recv_bounce, s, lock, _1, _2));
  }
  template<typename Stream> void
  SocketImpl<Stream>::onReadDemux(DestructionLock lock, boost::system::error_code erc)
  {
    if (erc)
    {
      if (onErrorFunc)
	onErrorFunc(erc);
      close();
    }
    else
    {
      if (onReadFunc)
	onReadFunc(readBuffer_);
      if (isConnected())
        read_or_recv(this, lock);
    }
  }


  template<typename Stream> void
  SocketImpl<Stream>::startReader()
  {
    read_or_recv(this, getDestructionLock());
  }

  template<typename Stream>
  template<typename Acceptor, typename BaseFactory> void
  SocketImpl<Stream>::onAccept(boost::system::error_code erc,Stream* s,
                               SocketFactory fact, Acceptor *a, BaseFactory bf)
  {
    if (erc)
      return;
    SocketImplBase* wrapper = dynamic_cast<SocketImplBase*>(bf(s));
    if (!wrapper)
    { // Failure.
      delete s;
    }
    else
    {
      // This is now connected.
      Socket* s = fact();
      s->setBase(wrapper);
      // Start reading.
      wrapper->startReader();
    }
    acceptOne(fact, a, bf);
  }

  template<typename Stream>
  template<typename Acceptor, typename BaseFactory> void
  SocketImpl<Stream>::acceptOne(SocketFactory fact, Acceptor *a, BaseFactory bf)
  {
    Stream* s = new Stream(get_io_service());
    a->async_accept(*s,
	boost::bind(&SocketImpl<Stream>::template onAccept<Acceptor, BaseFactory>
	  , _1, s, fact, a, bf));
  }


  template<typename Stream>BaseSocket*
  SocketImpl<Stream>::create(Stream*s)
  {
    SocketImpl<Stream>* bs = new SocketImpl<Stream>;
    bs->base_ = s;
    return bs;
  }

  template<class Socket> inline void
  onTimer(boost::system::error_code erc, Socket& s)
  {
    // If timer reached the end, connection timeout, interrupt.
    if (!erc)
      s.close();
    // Else we got aborted, do nothing
  }

  inline void onConnect(boost::system::error_code erc,
                        boost::asio::deadline_timer & timer,
                        libport::Semaphore& sem,
                        boost::system::error_code& caller_erc)
  {
    caller_erc = erc;
    // If timer was allready canceled, cancel is a noop and does no harm.
    timer.cancel();
    sem++;
  }

#ifndef LIBPORT_NO_SSL
  template<typename Stream>
  BaseSocket* SSLLayer(SSLSettings settings, Stream* s)
  {
    using namespace boost::asio;
    boost::system::error_code erc;
    ssl::context context(s->get_io_service(),
	boost::asio::ssl::context::sslv23_server);
    context.set_options(settings.options);
    if (!settings.cipherList.empty())
    {
      SSL_CTX* ctx = context.impl();
      if (!SSL_CTX_set_cipher_list(ctx, settings.cipherList.c_str()))
	throw std::runtime_error("SSL_CTX_set_cipher_list failed");
    }

#define comma ,
#define CHECK_CALL(param, call, args)  do \
    if (!param.empty()) { call(param, args erc); if (erc) throw erc;} while(0)

    CHECK_CALL(settings.tmpDHFile, context.use_tmp_dh_file, );
    CHECK_CALL(settings.certChainFile, context.use_certificate_chain_file, );
    CHECK_CALL(settings.privateKeyFile, context.use_private_key_file,
	ssl::context_base::pem comma);
#undef CHECK_CALL

    // Create a SSL stream taking its underlying stream by ref.
    typedef boost::asio::ssl::stream<Stream&> SSLStream;
    SSLStream *sslStream = new SSLStream(*s, context);
    sslStream->handshake(sslStream->server, erc);
    if (erc)
    {
      delete sslStream;
      return 0;
    }
    BaseSocket* bs = SocketImpl<SSLStream>::create(sslStream);
    // s ownership is taken: mark for deletion when bs dies.
    bs->deletor << boost::bind(deletor<Stream>, s);
    return bs;
  }
#endif
} //namespace netdetail


inline void
Socket::setBase(BaseSocket* b)
{
  base_ = b;
  base_-> onReadFunc = boost::bind(&Socket::onRead_, this, _1);
  base_->onErrorFunc = boost::bind(&Socket::onError, this, _1);

}


inline bool
Socket::onRead_(boost::asio::streambuf& buf)
{
  // Dump the stream in our linear buffer
  std::istream is(&buf);
  static const int blockSize = 1024;
  while (is.good())
  {
    int oldBufferLength = buffer.length();
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


template<typename Proto> typename Proto::endpoint
resolve(const std::string& host,
       const std::string& port, boost::system::error_code &erc)
{
  typename Proto::resolver::query query(host, port);
  typename Proto::resolver resolver(get_io_service());
  typename Proto::resolver::iterator iter = resolver.resolve(query, erc);
  if (erc)
    return typename Proto::endpoint();
  return *iter;
}

template<class Proto, class BaseFactory> boost::system::error_code
Socket::connectProto(const std::string& host, const std::string& port,
                     utime_t timeout, BaseFactory bf)
{
  using namespace netdetail;
  boost::system::error_code erc;
  typename Proto::endpoint ep = resolve<Proto>(host, port, erc);
  if (erc)
    return erc;
  typename Proto::socket* s = new typename Proto::socket(get_io_service());
  if (!timeout)
    s->connect(ep, erc);
  else
  {
    boost::asio::deadline_timer timer(get_io_service());
    libport::Semaphore sem;
    s->async_connect(ep, boost::bind(&netdetail::onConnect, _1,
                                     boost::ref(timer),
                                     boost::ref(sem),
                                     boost::ref(erc)));
    timer.expires_from_now(boost::posix_time::microseconds(timeout));
    timer.async_wait(boost::bind(&netdetail::onTimer<typename Proto::socket>,
                                 _1, boost::ref(*s)));
    sem--;
  }
  if (erc)
    return erc;
  SocketImplBase* newS = dynamic_cast<SocketImplBase*>(bf(s));
  if (!newS)
  {
    delete s;
#if BOOST_VERSION >= 103600
    return boost::system::errc::make_error_code(boost::system::errc::operation_canceled);
#else
    return boost::system::posix_error::make_error_code(boost::system::posix_error::operation_canceled);
#endif
}
  setBase(newS);
  // Start reading.
  newS->startReader();
  return erc;
}

inline boost::system::error_code
Socket::connect(const std::string& host, const std::string& port, bool udp,
                utime_t timeout)
{
  if (udp)
    return connectProto<boost::asio::ip::udp>(host, port, timeout,
	&netdetail::SocketImpl<boost::asio::ip::udp::socket>::create);
  else
    return connectProto<boost::asio::ip::tcp>(host, port, timeout,
	&netdetail::SocketImpl<boost::asio::ip::tcp::socket>::create);
}


template<typename Proto, typename BaseFactory> boost::system::error_code
Socket::listenProto(SocketFactory f, const std::string& host,
    const std::string& port, BaseFactory bf)
{
  boost::system::error_code erc;
  typename Proto::endpoint ep = resolve<Proto>(host, port, erc);
  if (erc)
    return erc;
  typename Proto::acceptor* a = new typename Proto::acceptor(get_io_service(), ep);
  BaseSocket* impl = new netdetail::AcceptorImpl<typename Proto::acceptor>(a);
  setBase(impl);
  netdetail::SocketImpl<typename Proto::socket>::acceptOne(f, a, bf);
  return erc;
}

inline
boost::system::error_code
Socket::listen(SocketFactory f, const std::string& host,
               const std::string& port, bool udp)
{
  (void)udp;
  assert(!udp);
  boost::system::error_code erc;
  erc = Socket::listenProto<boost::asio::ip::tcp>(f, host, port,
      &netdetail::SocketImpl<boost::asio::ip::tcp::socket>::create);
  return erc;
}

inline
Socket::Handle
Socket::listenUDP(const std::string& host, const std::string& port,
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
    erc.assign(boost::system::posix_error::bad_address,
               boost::system::get_posix_category());
  return Handle();
ok:
  s->start_receive();
  return Handle();
}

#ifndef LIBPORT_NO_SSL
inline boost::system::error_code
Socket::listenSSL(SocketFactory f, const std::string& host,
    const std::string&  port,
    boost::asio::ssl::context_base::method ctx,
    boost::asio::ssl::context::options options,
    const std::string& privateKeyFile,
    const std::string& certChainFile,
    const std::string& tmpDHFile,
    const std::string& cipherList)
{
  netdetail::SSLSettings settings;
  settings.context = ctx;
  settings.options = options;
  settings.privateKeyFile = privateKeyFile;
  settings.certChainFile = certChainFile;
  settings.tmpDHFile = tmpDHFile;
  settings.cipherList = cipherList;
  boost::system::error_code erc;
  erc = listenProto<boost::asio::ip::tcp>(f, host, port,
      (boost::function1<BaseSocket*, boost::asio::ip::tcp::socket*>)boost::bind(&netdetail::SSLLayer<boost::asio::ip::tcp::socket>, settings, _1));
  return erc;
}
#endif

inline
Socket::~Socket()
{
  wasDestroyed();
  if (base_)
  {
    Destructible::DestructionLock l = base_->getDestructionLock();
    base_->close();
    base_->destroy();
    base_->onReadFunc = 0;
    base_->onErrorFunc = 0;
    base_ = 0;
  }
}

inline void
Socket::destroy()
{
  // Lock in case the user calls destroy() in its error handler.
  Destructible::DestructionLock l = getDestructionLock();
  // It is safe to reach that point with an open socket.
  close();
  AsioDestructible::destroy();
}

inline void
AsioDestructible::doDestroy()
{

  get_io_service().post(boost::bind(
	netdetail::deletor<AsioDestructible>, this));
}



inline boost::asio::io_service&
get_io_service(bool startWorkerThread)
{
  static boost::asio::io_service* io = 0;
  if (!io)
  {
    io = new boost::asio::io_service;
    if (startWorkerThread)
      libport::startThread(boost::bind(&netdetail::runIoService, io));
  }
  return *io;
}
}
