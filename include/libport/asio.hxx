/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#if defined LIBPORT_ENABLE_SSL
// There are many warnings in asio/ssl.  We support GCC 4.1 which does
// not supproted "#pragma GCC diagnostics", so we have to disable all
// the warnings in this header.  The cure might be worse than the
// disease, and maybe fixing the headers on our machines would be a
// better option.
# if defined __GNUC__
#  pragma GCC system_header
# endif
#endif

#include <libport/system-warning-push.hh>
#  include <boost/asio.hpp>
#  include <libport/bind.hh>
#  include <boost/system/error_code.hpp>
#  include <boost/lambda/lambda.hpp>
#  include <boost/lambda/construct.hpp>
#include <libport/system-warning-pop.hh>

#include <libport/lexical-cast.hh>
#include <libport/lockable.hh>
#include <libport/semaphore.hh>
#include <libport/thread.hh>
#include <libport/unistd.h>

namespace libport
{

  namespace netdetail
  {
    using boost::asio::io_service;
    namespace errorcodes =
# if 103600 <= BOOST_VERSION
    boost::system::errc;
# else
    boost::system::posix_error;
# endif
    typedef ::libport::Socket::SocketFactory SocketFactory;

    typedef boost::asio::basic_datagram_socket
               <boost::asio::ip::udp,
                boost::asio::datagram_socket_service<boost::asio::ip::udp> >
            udpsock;

    class LIBPORT_API SocketImplBase
      : public BaseSocket
      , protected libport::Lockable
    {
    public:
      SocketImplBase()
        : current_(-1)
        , pending_(false)
      {}

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

    template<typename Stream, typename Lock>
    void
    read_or_recv(SocketImpl<Stream>* s, Lock lock);

    template<class Stream>
    class SocketImpl : public SocketImplBase
    {
    public:
      SocketImpl()
        : base_(0)
      {}
      ~SocketImpl()
      {
        wasDestroyed();
        waitForDestructionPermission();
        delete base_;
      }

      void write(const void* data, size_t length);
      void close();

      unsigned short getRemotePort() const;
      std::string getRemoteHost() const;
      unsigned short getLocalPort() const;
      std::string getLocalHost() const;
      bool isConnected() const;

      template<typename Acceptor, typename BaseFactory>
      static void
      onAccept(io_service& io, boost::system::error_code erc, Stream* s,
               SocketFactory fact, Acceptor *a, BaseFactory bf);

      template<typename Acceptor, typename BaseFactory>
      static void
      acceptOne(io_service& io, SocketFactory fact, Acceptor *a,
                BaseFactory bf);

      static BaseSocket* create(Stream* base);
      void startReader();
#if ! defined WIN32
      int stealFD();
      int getFD();
#endif
    private:
      Stream* base_;
      void continueWrite(DestructionLock lock, boost::system::error_code erc,
			 size_t sz);
      void onReadDemux(DestructionLock lock, boost::system::error_code erc);

      friend class libport::Socket;

      template<class T, class L>
        friend void read_or_recv(SocketImpl<T>*, L);

      template<class T>
        friend void send_bounce(SocketImpl<T>*,const void*, size_t);

      friend void
      recv_bounce(SocketImpl<udpsock>*s, AsioDestructible::DestructionLock lock,
		  boost::system::error_code erc, size_t recv);
      std::vector<char> udpBuffer_;
    };

    // Acceptor implementation
    template<class Acceptor>
    class AcceptorImpl
      : public BaseSocket
    {
    public:
      AcceptorImpl(Acceptor* base);
      ~AcceptorImpl();
      bool isConnected() const
      {
        return false;
      }
      void close();
#define ACCEPTOR_FAIL                                                   \
    {throw std::runtime_error("Call not implemented for Acceptors");}
      void write(const void*, size_t) ACCEPTOR_FAIL
      unsigned short getRemotePort() const ACCEPTOR_FAIL
      std::string getRemoteHost() const ACCEPTOR_FAIL
#undef ACCEPTOR_FAIL
      unsigned short getLocalPort() const;
      std::string getLocalHost() const;
      int stealFD()
      {
        return -1;
      }
      int getFD();
    private:
      Acceptor* base_;
    };

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

    template<class Acceptor>
    unsigned short
    AcceptorImpl<Acceptor>::getLocalPort() const
    {
      return base_->local_endpoint().port();
    }

    template<class Acceptor>
    std::string
    AcceptorImpl<Acceptor>::getLocalHost() const
    {
      return base_->local_endpoint().address().to_string();
    }

    template<class Acceptor>
    int
    AcceptorImpl<Acceptor>::getFD()
    {
      return base_->native();
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
      virtual void reply(const void* data, size_t length);
    private:
      boost::asio::ip::udp::socket& socket_;
      boost::asio::ip::udp::endpoint endpoint_;
      Destructible::DestructionLock destructionLock_;
    };

    class UDPSocket: public Destructible
    {
    public:
      UDPSocket();
      typedef boost::function3<void, const void*, size_t,
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
      socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_),
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

#if defined LIBPORT_ENABLE_SSL
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
    UDPLinkImpl::reply(const void* data, size_t length)
    {
      socket_.send_to(boost::asio::buffer(data, length), endpoint_);
    }

    // FIXME: use the delete_ptr in boost::lambda.
    template<class T>
    void
    deletor(T* ptr)
    {
      delete ptr;
    }

    // Bind the Socket, the SocketImpl, and the boost socket together.
    template<class Sock, class Factory>
    SocketImplBase*
    bind_or_delete(Socket* sock, Factory f, Sock* s)
    {
      if (SocketImplBase* base =  static_cast<SocketImplBase*>(f(s)))
      {
        sock->setBase(base);
        return base;
      }
      else
      {
        delete s;
        return 0;
      }
    }

    template<typename Stream>
    void
    SocketImpl<Stream>::close()
    {
      Destructible::DestructionLock l = getDestructionLock();
      if (base_->lowest_layer().is_open())
      {
        // Error code is ignored on purpose, after all, we're closing.
        boost::system::error_code erc;
        base_->lowest_layer().shutdown(
          Stream::lowest_layer_type::shutdown_both,
          erc);
        base_->lowest_layer().close(erc);
      }
    }

    template<typename Stream>
    unsigned short
    SocketImpl<Stream>::getRemotePort() const
    {
      return base_->lowest_layer().remote_endpoint().port();
    }

    template<typename Stream>
    std::string
    SocketImpl<Stream>::getRemoteHost() const
    {
      return base_->lowest_layer().remote_endpoint().address().to_string();
    }

    template<typename Stream>
    unsigned short
    SocketImpl<Stream>::getLocalPort() const
    {
      return base_->lowest_layer().local_endpoint().port();
    }

    template<typename Stream>
    std::string
    SocketImpl<Stream>::getLocalHost() const
    {
      return base_->lowest_layer().local_endpoint().address().to_string();
    }

    template<typename Stream>
    bool
    SocketImpl<Stream>::isConnected() const
    {
      return base_->lowest_layer().is_open();
    }

#if ! defined WIN32
    template<typename Stream>
    int
    SocketImpl<Stream>::stealFD()
    {
      size_t fd = base_->lowest_layer().native();
      fd = dup(fd);
      // Call close, so shutdown wont be called
      base_->lowest_layer().close();
      return fd;
    }

    template<typename Stream>
    int
    SocketImpl<Stream>::getFD()
    {
      return base_->lowest_layer().native();
    }
#endif

    template<class T>
    void
    send_bounce(SocketImpl<T>* s,const void* buffer, size_t length)
    {
      libport::BlockLock bl(s);
      std::ostream stream(&s->buffers_[s->current_==-1 ? 0:1-s->current_]);
      stream.write((const char*)buffer, std::streamsize(length));
      s->pending_ = true;
      if (s->current_ == -1)
      {
        s->current_ = 1; // We wrote on 0, continueWrite will swap.
        s->continueWrite(s->getDestructionLock(),
                         boost::system::error_code(),
                         0);
      }
    }

    inline void
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

    template<typename Stream>
    void
    SocketImpl<Stream>::write(const void* buffer, size_t length)
    {
      send_bounce(this, buffer, length);
    }

    template<typename Stream>
    void
    SocketImpl<Stream>::continueWrite(DestructionLock lock,
                                      boost::system::error_code er,
                                      size_t)
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
        boost::asio::async_write(
          *base_, buffers_[current_],
          boost::bind(&SocketImpl<Stream>::continueWrite,
                      this, lock,  _1, _2));
      else
      {
        current_ = -1;
      }
      pending_ = false;
    }


    template<typename Stream, typename Lock>
    void
    read_or_recv(SocketImpl<Stream>* s,
                 Lock lock)
    {
      boost::asio::async_read(*s->base_, s->readBuffer_,
                              boost::asio::transfer_at_least(1),
                              boost::bind(&SocketImpl<Stream>::onReadDemux,
                                          s, lock, _1));
    }

    inline void
    recv_bounce(SocketImpl<udpsock>*s,
                SocketImpl<udpsock>::DestructionLock lock,
                boost::system::error_code erc, size_t recv)
    {
      std::ostream stream(&s->readBuffer_);
      stream.write(&s->udpBuffer_[0], std::streamsize(recv));
      s->onReadDemux(lock, erc);
    }

    template<>
    inline void
    read_or_recv(SocketImpl<udpsock>*s,
                 SocketImpl<udpsock>::DestructionLock lock)
    {
      s->udpBuffer_.resize(2000);
      s->base_->async_receive(boost::asio::buffer(&s->udpBuffer_[0], 2000),
                              boost::bind(&recv_bounce, s, lock, _1, _2));
    }

    template<typename Stream>
    void
    SocketImpl<Stream>::onReadDemux(DestructionLock lock,
                                    boost::system::error_code erc)
    {
      BlockLock bl(callbackLock);
      if (erc)
      {
        if (onErrorFunc)
          onErrorFunc(erc);
        close();
        // We no longuer need our owner socket to stay alive, we wont call it.
        unlinkAll();
      }
      else
      {
        if (onReadFunc)
          onReadFunc(readBuffer_);
        if (isConnected())
          read_or_recv(this, lock);
      }
    }

    template<typename Stream>
    void
    SocketImpl<Stream>::startReader()
    {
      read_or_recv(this, getDestructionLock());
    }

    template<typename Stream>
    template<typename Acceptor, typename BaseFactory>
    void
    SocketImpl<Stream>::onAccept(
                                 io_service& io,
                                 boost::system::error_code erc,
                                 Stream* s,
                                 SocketFactory fact,
                                 Acceptor *a,
                                 BaseFactory bf)
    {
      if (erc)
        return;
      if (SocketImplBase* wrapper = dynamic_cast<SocketImplBase*>(bf(s)))
      {
        // This is now connected.
        Socket* s = fact();
        s->setBase(wrapper);
        s->onConnect();
        // Start reading.
        wrapper->startReader();
      }
      else
      {
        // Failure.
        delete s;
      }
      acceptOne(io, fact, a, bf);
    }

    template<typename Stream>
    template<typename Acceptor, typename BaseFactory>
    void
    SocketImpl<Stream>::acceptOne(io_service& io, SocketFactory fact,
                                  Acceptor *a,
                                  BaseFactory bf)
    {
      Stream* s = new Stream(io);
      a->async_accept(
        *s,
        boost::bind(&SocketImpl<Stream>::template
                    onAccept<Acceptor, BaseFactory>,
                    boost::ref(io), _1, s, fact, a, bf));
    }


    template<typename Stream>
    BaseSocket*
    SocketImpl<Stream>::create(Stream*s)
    {
      SocketImpl<Stream>* bs = new SocketImpl<Stream>;
      bs->base_ = s;
      return bs;
    }

    template<class Socket>
    inline void
    onTimer(boost::system::error_code erc,
            Socket& s,
            libport::Semaphore& sem,
            Destructible::DestructionLock)
    {
      // If timer reached the end, connection timeout, interrupt.
      if (!erc)
        s.close();
      // Else we got aborted, do nothing
      sem++;
    }

    inline void
    onConnect(boost::system::error_code erc,
              boost::asio::deadline_timer & timer,
              libport::Semaphore& sem,
              boost::system::error_code& caller_erc)
    {
      caller_erc = erc;
      // If timer was allready canceled, cancel is a noop and does no harm.
      timer.cancel();
      sem++;
    }

#if defined LIBPORT_ENABLE_SSL
    template<typename Stream>
    BaseSocket*
    SSLLayer(SSLSettings settings, Stream* s)
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

#define ASIO_COMMA ,
#define CHECK_CALL(param, call, args)           \
      do {                                      \
        if (!param.empty())                     \
        {                                       \
          call(param, args erc);                \
          if (erc)                              \
            throw erc;                          \
        }                                       \
      } while(0)

      CHECK_CALL(settings.tmpDHFile, context.use_tmp_dh_file, );
      CHECK_CALL(settings.certChainFile, context.use_certificate_chain_file, );
      CHECK_CALL(settings.privateKeyFile, context.use_private_key_file,
                 ssl::context_base::pem ASIO_COMMA);
#undef CHECK_CALL
#undef ASIO_COMMA

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

  namespace netdetail
  {
    template<typename Proto, class BaseFactory>
    void
    async_connect(typename Proto::socket* bs,
                  Socket* s,
                  Destructible::DestructionLock,
                  BaseFactory bf,
                  const boost::system::error_code& error)
    {
      if (error)
      {
        s->onError(error);
        delete bs;
      }
      else
      {
        SocketImplBase* b = bind_or_delete(s, bf, bs);
        if (!b)
          s->onError(make_error_code(errorcodes::operation_canceled));
        b->link(s->getDestructionLock());
        b->startReader();
        s->onConnect();
      }
    }

    template<typename Proto, class BaseFactory>
    void
    async_resolve(typename Proto::resolver* r,
                  Socket* s,
                  Destructible::DestructionLock l,
                  BaseFactory bf,
                  boost::system::error_code error,
                  const typename Proto::resolver::iterator i)
    {
      if (error)
        s->onError(error);
      else
      {
        typename Proto::endpoint ep = *i;
        typename Proto::socket* bs =
          new typename Proto::socket(r->get_io_service());
        bs->async_connect(ep,
                          boost::bind(&async_connect<Proto, BaseFactory>,
                                      bs, s, l, bf, _1));
      }
      delete r;
    }

    template<typename Proto, class BaseFactory>
    void
    async_resolve_connect(Socket* s,
                          Destructible::DestructionLock l,
                          const std::string& host,
                          const std::string& port,
                          int,
                          BaseFactory bf)
    {
      typename Proto::resolver::query query(host, port);
      typename Proto::resolver* resolver =
        new typename Proto::resolver(s->get_io_service());
      resolver->async_resolve(query,
                              boost::bind(&async_resolve<Proto, BaseFactory>,
                                          resolver, s, l, bf, _1, _2));
    }

  }

  template<typename Proto>
  typename Proto::endpoint
  resolve(const std::string& host,
          const std::string& port,
          boost::system::error_code &erc)
  {
    typename Proto::resolver::query query(host, port);
    // Synchronous resolution, we do not care which io_service is used.
    typename Proto::resolver resolver(get_io_service());
    typename Proto::resolver::iterator iter = resolver.resolve(query, erc);
    if (erc)
      return typename Proto::endpoint();
    return *iter;
  }

  inline
  boost::asio::io_service&
  Socket::get_io_service()
  {
    return io_;
  }

  template<class Proto, class BaseFactory>
  boost::system::error_code
  Socket::connectProto(const std::string& host,
                       const std::string& port,
                       useconds_t timeout,
                       bool async,
                       BaseFactory bf)
  {
    using namespace netdetail;
    boost::system::error_code erc;
    if (async)
    {
      netdetail::async_resolve_connect<Proto, BaseFactory>
        (this,
         this->getDestructionLock(),
         host, port, timeout, bf);
      return erc;
    }
    typename Proto::endpoint ep = resolve<Proto>(host, port, erc);
    if (erc)
      return erc;
    typename Proto::socket* s = new typename Proto::socket(get_io_service());
    SocketImplBase* newS = 0;
    if (!timeout)
    {
      s->connect(ep, erc);
      if (erc)
      {
        delete s;
        return erc;
      }
      newS = bind_or_delete(this, bf, s);
      if (!newS)
        return errorcodes::make_error_code(errorcodes::operation_canceled);
    }
    else
    {
      newS = bind_or_delete(this, bf, s);
      if (!newS)
        return errorcodes::make_error_code(errorcodes::operation_canceled);
      boost::asio::deadline_timer timer(get_io_service());
      libport::Semaphore sem;
      timer.expires_from_now(boost::posix_time::microseconds(timeout));
      timer.async_wait(boost::bind(&netdetail::onTimer<typename Proto::socket>,
                                   _1, boost::ref(*s),
                                   boost::ref(sem),
                                   newS->getDestructionLock()));
      s->async_connect(ep, boost::bind(&netdetail::onConnect, _1,
                                       boost::ref(timer),
                                       boost::ref(sem),
                                       boost::ref(erc)));
      sem--;
      sem--;
      if (erc)
        // Failure, but everything is bound and will get nicely
        // destroyed.
        return erc;
    }
    // Start reading.
    if (newS->isConnected())
    {
      // Do not die until newS is gone, since it will call our virtual
      // functions.
      newS->link(getDestructionLock());
      newS->startReader();
      onConnect();
    }
    return erc;
  }

  inline boost::system::error_code
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

  inline boost::system::error_code
  Socket::connect(const std::string& host,
                  unsigned port,
                  bool udp,
                  useconds_t timeout,
                  bool async)
  {
    return connect(host, string_cast(port), udp, timeout, async);
  }

  template<typename Proto, typename BaseFactory>
  boost::system::error_code
  Socket::listenProto(SocketFactory f,
                      const std::string& host,
                      const std::string& port,
                      BaseFactory bf)
  {
    boost::system::error_code erc;
    typename Proto::endpoint ep = resolve<Proto>(host, port, erc);
    if (erc)
      return erc;
    typename Proto::acceptor* a;
    try {
      a = new typename Proto::acceptor(get_io_service(), ep);
    }
    catch(const boost::system::system_error& se)
    {
      return se.code();
    }
    BaseSocket* impl =
      new netdetail::AcceptorImpl<typename Proto::acceptor>(a);
    setBase(impl);
    netdetail::SocketImpl<typename Proto::socket>::acceptOne(
      get_io_service(), f, a, bf);
    return erc;
  }

  inline boost::system::error_code
  Socket::listen(SocketFactory f,
                 const std::string& host,
                 const std::string& port,
                 bool udp)
  {
    (void)udp;
    assert(!udp);
    boost::system::error_code erc;
    erc = Socket::listenProto<boost::asio::ip::tcp>(
      f, host, port,
      &netdetail::SocketImpl<boost::asio::ip::tcp::socket>::create);
    return erc;
  }

  inline boost::system::error_code
  Socket::listen(SocketFactory f,
                 const std::string& host,
                 unsigned port,
                 bool udp)
  {
    return listen(f, host, string_cast(port), udp);
  }



#if defined LIBPORT_ENABLE_SSL
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
    erc = listenProto<boost::asio::ip::tcp>(
      f, host, port,
      (boost::function1<BaseSocket*, boost::asio::ip::tcp::socket*>)
      boost::bind(&netdetail::SSLLayer<boost::asio::ip::tcp::socket>,
                  settings, _1));
    return erc;
  }
#endif



  template<class Sock>
  inline void
  Socket::setFD(int fd, typename Sock::protocol_type proto)
  {
    if (base_ && base_->isConnected())
    {
      base_->close();
      base_->destroy();
      base_ = 0;
    }
    Sock* s = new Sock(get_io_service());
    s->assign(proto, fd);
    netdetail::SocketImplBase* b =
      (netdetail::SocketImplBase*)netdetail::SocketImpl<Sock>::create(s);
    setBase(b);
    b->startReader();
  }

  inline void
  AsioDestructible::doDestroy()
  {
    get_io_service().post(boost::bind(netdetail::deletor<AsioDestructible>,
                                      this));
  }

  inline void
  Socket::onError(boost::system::error_code)
  {
    // Nothing
  }

  namespace netdetail
  {

    inline void
    timer_trigger(boost::shared_ptr<boost::asio::deadline_timer>,
                  boost::function0<void> callback,
                  boost::system::error_code erc)
    {
      if (!erc)
        callback();
    }

  }

  inline AsyncCallHandler
  asyncCall(boost::function0<void> callback, useconds_t usDelay,
            boost::asio::io_service& io)
  {
    AsyncCallHandler
      res(new boost::asio::deadline_timer(io));
    res->expires_from_now(boost::posix_time::microseconds(usDelay));
    res->async_wait(boost::bind(&netdetail::timer_trigger, res, callback, _1));
    return res;
  }

}
