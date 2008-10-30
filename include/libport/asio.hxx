#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/construct.hpp>

#include <libport/thread.hh>
#include <libport/lockable.hh>

namespace libport {
namespace netdetail {

  typedef ::libport::Socket::SocketFactory SocketFactory;
  class SocketImplBase: public BaseSocket, private libport::Lockable
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
      friend class Socket;
      template<class Stream> friend class SocketImpl;
  };
  template<class Stream> class SocketImpl
    : public SocketImplBase
  {
    public:
      SocketImpl():base_(0) {}
      ~SocketImpl() { delete base_;}
      void write(const void* data, unsigned int length);
      void close();
      bool isConnected();
      template<typename Acceptor, typename BaseFactory> static void
        onAccept(Stream* s, SocketFactory fact, Acceptor *a, BaseFactory bf);
      template<typename Acceptor, typename BaseFactory> static void
        acceptOne(SocketFactory fact, Acceptor *a, BaseFactory bf);
      static BaseSocket* create(Stream* base);
      void startReader();
    private:
      Stream* base_;
      void continueWrite(DestructionLock lock, boost::system::error_code erc, size_t sz);
      void onReadDemux(DestructionLock lock, boost::system::error_code erc);
      friend class Socket;
    };


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

  // FIXME: use the delete_ptr in boost::lambda.
  template<class T> void deletor(T* ptr)
  {
    delete ptr;
  }

  inline void
  runIoService(boost::asio::io_service* io)
  {
    // Trick to have io->run() never return.
    boost::asio::io_service::work work(*io);
    io->run();
  }

  inline boost::asio::io_service&
  get_io_service()
  {
    static boost::asio::io_service* io = 0;
    if (!io)
    {
      io = new boost::asio::io_service;
      libport::startThread(boost::bind(&runIoService, io));
    }
    return *io;
  }

  template<typename Stream> void
  SocketImpl<Stream>::close()
  {
    base_->lowest_layer().close();
  }

  template<typename Stream> bool
  SocketImpl<Stream>::isConnected()
  {
    return base_->lowest_layer().is_open();
  }

  template<typename Stream> void
  SocketImpl<Stream>::write(const void* buffer, unsigned int length)
  {
    libport::BlockLock bl(this);
    std::ostream stream(&buffers_[current_==-1 ? 0:1-current_]);
    stream.write((const char*)buffer, length);
    pending_ = true;
    if (current_ == -1)
    {
      current_ = 1; // We wrote on 0, continueWrite will swap.
      continueWrite(getDestructionLock(), boost::system::error_code(), 0 );
    }
  }

  template<typename Stream> void
  SocketImpl<Stream>::continueWrite(DestructionLock lock, boost::system::error_code er, size_t sz)
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
	boost::asio::async_read(*base_, readBuffer_,  boost::asio::transfer_at_least(1),
	    boost::bind(&SocketImpl<Stream>::onReadDemux,this, lock, _1));
    }
  }


  template<typename Stream> void
  SocketImpl<Stream>::startReader()
  {
    boost::asio::async_read(*base_, readBuffer_,  boost::asio::transfer_at_least(1),
       	boost::bind(&SocketImpl<Stream>::onReadDemux,this, getDestructionLock(), _1));
  }

  template<typename Stream>
  template<typename Acceptor, typename BaseFactory> void
  SocketImpl<Stream>::onAccept(Stream* s, SocketFactory fact, Acceptor *a, BaseFactory bf)
  {
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
	  , s, fact, a, bf));
  }


  template<typename Stream>BaseSocket*
  SocketImpl<Stream>::create(Stream*s)
  {
    SocketImpl<Stream>* bs = new SocketImpl<Stream>;
    bs->base_ = s;
    return bs;
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
  while (true)
  {
    int c = is.get();
    if (!is.good())
      break;
    buffer += (char)c;
  }
  // Call unread until it eats 0 characters.
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

template<class Proto, class BaseFactory> boost::system::error_code
Socket::connectProto(const std::string& host, const std::string& port, BaseFactory bf)
{
  using namespace netdetail;
  boost::system::error_code erc;
  typename Proto::resolver::query query(host, port);
  typename Proto::resolver resolver(netdetail::get_io_service());
  typename Proto::resolver::iterator iter = resolver.resolve(query,erc);
  if (erc)
    return erc;
  typename Proto::socket* s = new typename Proto::socket(get_io_service());
  s->connect(*iter, erc);
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
Socket::connect(const std::string& host, const std::string& port, bool udp)
{
  assert(!udp);
  /*
  if (udp)
    return connectProto<boost::asio::ip::udp>(host, port,
	&netdetail::SocketImpl<boost::asio::ip::udp::socket>::create);
  else
  */
    return connectProto<boost::asio::ip::tcp>(host, port,
	&netdetail::SocketImpl<boost::asio::ip::tcp::socket>::create);
}

template<typename Proto, typename BaseFactory> boost::system::error_code
Socket::listenProto(SocketFactory f, const std::string& host,
    const std::string& port, BaseFactory bf)
{
  boost::system::error_code erc;
  typename Proto::resolver::query query(host, port);
  typename Proto::resolver resolver(netdetail::get_io_service());
  typename Proto::resolver::iterator iter = resolver.resolve(query, erc);
  if (erc)
    return erc;
  typename Proto::acceptor* a = new typename Proto::acceptor(netdetail::get_io_service(), *iter);
  netdetail::SocketImpl<typename Proto::socket>::acceptOne(f, a, bf);
  return erc;
}

inline Socket::Handle
Socket::listen(SocketFactory f, const std::string& host,
    const std::string& port, boost::system::error_code & erc, bool udp)
{
  assert(!udp);
  erc = Socket::listenProto<boost::asio::ip::tcp>(f, host, port,
      &netdetail::SocketImpl<boost::asio::ip::tcp::socket>::create);
  return Handle();
}


#ifndef LIBPORT_NO_SSL
inline Socket::Handle
Socket::listenSSL(SocketFactory f, const std::string& host, const std::string&  port,
    boost::system::error_code& erc,
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
  erc = Socket::listenProto<boost::asio::ip::tcp>(f, host, port,
      (boost::function1<BaseSocket*, boost::asio::ip::tcp::socket*>)boost::bind(&netdetail::SSLLayer<boost::asio::ip::tcp::socket>, settings, _1));
  return Handle();
}
#endif

inline
Socket::~Socket()
{
  close();
  base_->destroy();
  base_->onReadFunc = 0;
  base_->onErrorFunc = 0;
  base_ = 0;
}

inline void
Socket::destroy()
{
  close();
  AsioDestructible::destroy();
}

inline void
AsioDestructible::doDestroy()
{
  netdetail::get_io_service().post(boost::bind(
	netdetail::deletor<AsioDestructible>, this));
}


}
