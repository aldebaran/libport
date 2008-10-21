#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/construct.hpp>

#include <libport/thread.hh>


namespace libport {
namespace netdetail {
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
template<class T> struct SocketFromAcceptor
{
  typedef typename T::protocol_type::socket type;
};
// FIXME: use the delete_ptr in boost::lambda.
template<class T> void deletor(T* ptr)
{
  delete ptr;
}
inline
void runIoService(boost::asio::io_service* io)
{
  // Trick to
  boost::asio::io_service::work work(*io);
  io->run();
}
inline boost::asio::io_service& get_io_service()
{
  static boost::asio::io_service* io = 0;
  if (!io)
  {
    io = new boost::asio::io_service;
    libport::startThread(boost::bind(&runIoService, io));
  }
  return *io;
}
template<typename SocketPtr> void
socketClose(SocketPtr s)
{
  s->lowest_layer().close();
}
template<typename SocketPtr> void
socketWrite(SocketPtr s, void* data, int sz)
{
  boost::asio::streambuf* request  = new boost::asio::streambuf();
  std::ostream request_stream(request);
  request_stream.write((char*)data, sz);
  boost::asio::async_write
  (*s, *request, 
   boost::bind(&deletor<boost::asio::streambuf>, request));
}
template<typename SocketPtr> bool
onReadDemux(BaseSocket* bs, SocketPtr, boost::asio::streambuf& buf,
            boost::system::error_code erc)
{
  if (erc)
  {
    bs->connected_ = false;
    std::cerr << "readdemux error " << erc.message() << std::endl;
    bs->onErrorFunc(erc);
    return false;
  }
  else
  {
    bs->onReadFunc(buf);
    return true;
  }
}
template<typename SocketPtr, typename Buf>
void readOne
(boost::system::error_code erc, SocketPtr s, 
 boost::function3<bool, SocketPtr, boost::asio::streambuf&,
 boost::system::error_code> onRead, 
 Buf buf
 )
{
  if (onRead(s, *buf, erc))
  {
    boost::asio::async_read(*s, *buf, boost::asio::transfer_at_least(1),
                            boost::bind(&readOne<SocketPtr, Buf>, _1, s, onRead, buf));
  }
}

template<typename SocketPtr> void
runReader
(SocketPtr s, 
 boost::function3<bool, SocketPtr, boost::asio::streambuf&,
 boost::system::error_code> onRead
 )
{
  typedef  boost::shared_ptr<boost::asio::streambuf> Buf;
  Buf buf(new boost::asio::streambuf);
  boost::asio::async_read(*s, *buf,
                          boost::asio::transfer_at_least(1),
                          boost::bind(&readOne<SocketPtr, Buf>, _1, s, onRead, buf));
}
template<typename SocketPtr> bool
onAccept(SocketPtr sock, SocketFactory fact)
{
  BaseSocket* s = fact();
  if (!s)
    return false;
  s->close = boost::bind(&socketClose<SocketPtr>, sock);
  s->write = boost::bind(&socketWrite<SocketPtr>, sock, _1, _2);
  runReader<SocketPtr>(sock, boost::bind(&onReadDemux<SocketPtr>, s, _1, _2, _3));
  return true;
}
#ifndef LIBPORT_NO_SSL
template<typename SocketPtr> bool
onAcceptSSL(SocketPtr sock, SocketFactory fact, SSLSettings settings)
{
  using namespace boost::asio;
  boost::system::error_code erc;
  ssl::context context(sock->get_io_service(),
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

  typedef boost::shared_ptr<ssl::stream<typename SocketPtr::element_type&> >
  SSLSocketPtr;
  SSLSocketPtr sslSock(
                       new boost::asio::ssl::stream<typename SocketPtr::element_type&>(*sock, context));
  
  sslSock->handshake(sslSock->server, erc);
  if (erc)
  {
    /// FIXME: add error reporting mechanism.
    std::cerr << "Handshake error " << erc.message() << std::endl;
    return true;
  }
  BaseSocket* s = fact();
  if (!s)
    return true;
  s->close = boost::bind(&socketClose<SSLSocketPtr>, sslSock);
  s->write = boost::bind(&socketWrite<SSLSocketPtr>, sslSock, _1, _2);
  
  // At the end of this function, sock will die. So keep a sharedptr on it,
  // and destroy it when the BaseSocket dies.
  SocketPtr* sockHandle = new SocketPtr(sock);
  s->deletor << boost::bind(&deletor<SocketPtr>, sockHandle);
  runReader<SSLSocketPtr>(sslSock, boost::bind(&onReadDemux<SSLSocketPtr>,
                                               s, _1, _2, _3));
  return true;
}
#endif
template<typename Acceptor, typename CB> void
acceptOne
(Acceptor& a, CB onAccept,
 boost::shared_ptr<typename netdetail::SocketFromAcceptor<Acceptor>::type> s,
 const boost::system::error_code& error)
{
  if (!error && onAccept(s))
  {
    boost::shared_ptr<typename SocketFromAcceptor<Acceptor>::type>
    sptr (new typename SocketFromAcceptor<Acceptor>::type(a.get_io_service()));
    a.async_accept(*sptr.get(),
                   boost::bind(&acceptOne<Acceptor, CB>, boost::ref(a), onAccept, sptr, _1));
  }
}

template<typename Acceptor, typename CB> void
runAcceptor(Acceptor& a, CB onAccept)
{
  boost::shared_ptr<typename SocketFromAcceptor<Acceptor>::type>
  sptr (new typename SocketFromAcceptor<Acceptor>::type(a.get_io_service()));
  a.async_accept(*sptr.get(), 
                 boost::bind(&acceptOne<Acceptor, CB>, boost::ref(a), onAccept, sptr, _1));
}

} //namespace netdetail

inline void
BaseSocket::destroy()
{
  netdetail::get_io_service().post(boost::bind(
                                        netdetail::deletor<BaseSocket>, this));
  //std::cerr <<"Done posting destroy message on " << (void*)this << std::endl;
}

inline
Socket::Socket()
{
  onReadFunc = boost::bind(&Socket::onRead_, this, _1);
  onErrorFunc = boost::bind(&Socket::onError, this, _1);
}

inline
bool
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

inline boost::system::error_code
BaseSocket::connect(const std::string& host, const std::string& port, bool)
{
  using namespace netdetail;
  typedef boost::shared_ptr<boost::asio::ip::tcp::socket> SocketPtr;
  SocketPtr ptr(new SocketPtr::element_type(netdetail::get_io_service()));
  boost::asio::ip::tcp::resolver::query query(host, port);
  boost::system::error_code erc;
  boost::asio::ip::tcp::resolver resolver(netdetail::get_io_service());
  boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query,erc);
  if (erc)
  {
    std::cerr << "Resolver error " << std::endl;
    return erc;
  }
  ptr->connect(*iter, erc);
  if (erc)
    return erc;
  this->close = boost::bind(&socketClose<SocketPtr>, ptr);
  this->write = boost::bind(&socketWrite<SocketPtr>, ptr, _1, _2);
  boost::function3<bool, SocketPtr, boost::asio::streambuf&,
     boost::system::error_code> f =  boost::bind(&onReadDemux<SocketPtr>, this, _1, _2, _3);
  netdetail::runReader<SocketPtr>(ptr,f);
  connected_ = true;
  return boost::system::error_code();
}

inline
Handle listenTCP(SocketFactory f, const std::string& , int port)
{
  boost::asio::io_service& io = netdetail::get_io_service();
  typedef boost::shared_ptr<boost::asio::ip::tcp::socket> SocketPtr;
  boost::asio::ip::tcp::acceptor* acceptor = new boost::asio::ip::tcp::acceptor
    (io,
     boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
  boost::function1<bool, SocketPtr> adapter = 
    boost::bind (&netdetail::onAccept<SocketPtr>, _1, f);
  netdetail::runAcceptor(*acceptor, adapter);
  return 0;
}

#ifndef LIBPORT_NO_SSL
inline
Handle listenSSL(SocketFactory f, const std::string& , int port,
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
  boost::asio::io_service& io = netdetail::get_io_service();
  typedef boost::shared_ptr<boost::asio::ip::tcp::socket> SocketPtr;
  boost::asio::ip::tcp::acceptor* acceptor = new boost::asio::ip::tcp::acceptor
   (io,
     boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
  boost::function1<bool, SocketPtr> adapter = 
    boost::bind (&netdetail::onAcceptSSL<SocketPtr>, _1, f, settings);
  netdetail::runAcceptor(*acceptor, adapter);
  return 0;
}
#endif
inline
Handle listen(SocketFactory f, const std::string& host, int port, bool udp)
{
 if (!udp)
   return listenTCP(f, host, port);
 else
   return 0;
}

}
