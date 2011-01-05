/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#if defined LIBPORT_ENABLE_SSL
// There are many warnings in asio/ssl.  We support GCC 4.1 which does
// not support "#pragma GCC diagnostics", so we have to disable all
// the warnings in this header.  The cure might be worse than the
// disease, and maybe fixing the headers on our machines would be a
// better option.
# if defined __GNUC__
#  pragma GCC system_header
# endif
#endif

#include <libport/asio.hh>
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

    template<class Stream> class SocketImpl;

    template<typename Stream, typename Lock>
    void
    read_or_recv(SocketImpl<Stream>* s, Lock lock);

    void
    onConnect(boost::system::error_code erc,
              boost::asio::deadline_timer & timer,
              libport::Semaphore& sem,
              boost::system::error_code& caller_erc);

    template<class Stream>
    class SocketImpl : public SocketImplBase
    {
    public:
      SocketImpl()
        : base_(0)
        , bytesReceived_(0)
        , bytesSent_(0)
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
      void syncWrite(const void* data, size_t length);
      // Bounce to read_dispatch
      std::string read(size_t length);
      bool isConnected() const;
      unsigned long bytesReceived() const
      {
        return bytesReceived_;
      }
      unsigned long bytesSent() const
      {
        return bytesSent_;
      }
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
#endif
      native_handle_type getFD();
      // Effective backend
      std::string read_(size_t length);
    protected:
      virtual void doDestroy();
    private:
      Stream* base_;
      void continueWrite(DestructionLock lock, boost::system::error_code erc,
			 size_t sz);
      void onReadDemux(DestructionLock lock, boost::system::error_code erc,
                       size_t);

      friend class libport::Socket;

      template<class T, class L>
        friend void read_or_recv(SocketImpl<T>*, L);

      template<class T>
        friend void send_bounce(SocketImpl<T>*,const void*, size_t);

      friend void
      recv_bounce(SocketImpl<udpsock>*s, AsioDestructible::DestructionLock lock,
		  boost::system::error_code erc, size_t recv);
      std::vector<char> udpBuffer_;
      unsigned long bytesReceived_;
      unsigned long bytesSent_;
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
      std::string read(size_t) ACCEPTOR_FAIL
      unsigned short getRemotePort() const ACCEPTOR_FAIL
      std::string getRemoteHost() const ACCEPTOR_FAIL
      void startReader() ACCEPTOR_FAIL
      void syncWrite(const void*, size_t) ACCEPTOR_FAIL
      unsigned long bytesSent() const ACCEPTOR_FAIL
      unsigned long bytesReceived() const ACCEPTOR_FAIL
#undef ACCEPTOR_FAIL
      unsigned short getLocalPort() const;
      std::string getLocalHost() const;
      native_handle_type stealFD()
      {
        return invalid_handle;
      }
      native_handle_type getFD();
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
    native_handle_type
    AcceptorImpl<Acceptor>::getFD()
    {
      return base_->native();
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
    void SocketImpl<Stream>::doDestroy()
    {
      base_->get_io_service().post(
            boost::bind(netdetail::deletor<SocketImpl<Stream> >,this));
    }

    template<typename Stream>
    void syncWriteDispatch(Stream* s, const void* data, size_t length)
    {
      boost::asio::write(*s, boost::asio::buffer(data, length));
    }

    template<> inline
    void syncWriteDispatch<boost::asio::ip::udp::socket>
    (boost::asio::ip::udp::socket* s, const void* data, size_t length )
    {
      s->send(boost::asio::buffer(data, length));
    }

    template<typename Stream>
    void
    SocketImpl<Stream>::syncWrite(const void* data, size_t length)
    {
      syncWriteDispatch(base_, data, length);
      bytesSent_ += length;
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

    class transfer_exactly {
    public:
      transfer_exactly(size_t amount): amount_(amount) {}
      size_t operator()(const boost::system::error_code& error,
                        size_t transferred)
      {
        if (error)
          return 0;
        //std::cerr << "txf " << amount_ <<" " << transferred
        //<< " => " << amount_ - transferred << std::endl;
        aver(amount_>= transferred);
        return amount_ - transferred;
      }
    private:
      // Amount left to transfer.
      size_t amount_;
    };

    template<typename Stream>
    std::string
    SocketImpl<Stream>::read_(size_t length)
    {
      std::string buffer;
      boost::asio::read(*base_, readBuffer_,
                        transfer_exactly(length));
      std::istream is(&readBuffer_);
      buffer.resize(length);
      is.read(&buffer[0], length);
      long len = is.gcount();
      buffer.resize(len);
      bytesReceived_ += length;
      return buffer;
    }

    template<typename T>
    std::string
    read_dispatch(SocketImpl<T>* sock, size_t length)
    {
      return sock->read_(length);
    }

    template<>
    inline std::string
    read_dispatch<boost::asio::ip::udp::socket>(
                                                SocketImpl<boost::asio::ip::udp::socket>*,
                                            size_t)
    {
      throw std::runtime_error("Not implemented");
    }

    template<typename Stream>
    std::string
    SocketImpl<Stream>::read(size_t length)
    {
      return read_dispatch<Stream>(this, length);
    }

#if ! defined WIN32
    template<typename Stream>
    native_handle_type
    SocketImpl<Stream>::stealFD()
    {
      size_t fd = base_->lowest_layer().native();
      fd = dup(fd);
      // Call close, so shutdown wont be called
      base_->lowest_layer().close();
      return fd;
    }
#endif

    template<typename Stream>
    native_handle_type
    SocketImpl<Stream>::getFD()
    {
      // We need the C-cast here, because underlying type changes with Stream.
      return (native_handle_type)base_->lowest_layer().native();
    }


    template<class T>
    void
    send_bounce(SocketImpl<T>* s,const void* buffer, size_t length)
    {
      libport::BlockLock bl(s);
      s->buffers_[s->current_==-1 ? 0:1-s->current_]
         .sputn((const char*)buffer, length);
      s->pending_ = true;
      if (s->current_ == -1)
      {
        s->current_ = 1; // We wrote on 0, continueWrite will swap.
        s->continueWrite(s->getDestructionLock(),
                         boost::system::error_code(),
                         0);
      }
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
                                      size_t sz)
    {
      if (er)
      {
        if (onErrorFunc)
          onErrorFunc(er);
        close();
        return;
      }
      else
        bytesSent_ += sz;
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
                                          s, lock, _1, _2));
    }
    template<typename Stream>
    void
    SocketImpl<Stream>::onReadDemux(DestructionLock lock,
                                    boost::system::error_code erc,
                                    size_t sz)
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
        bytesReceived_ += sz;
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
          boost::system::error_code &erc,
          boost::asio::io_service& ios)
  {
    typename Proto::resolver::query query(host, port);
    // Synchronous resolution, we do not care which io_service is used.
    typename Proto::resolver resolver(ios);
    typename Proto::resolver::iterator iter = resolver.resolve(query, erc);
    if (erc)
      return typename Proto::endpoint();
    return *iter;
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
    typename Proto::endpoint ep = resolve<Proto>(host, port, erc,
                                                 get_io_service());
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
  template<typename Proto, typename BaseFactory>
  boost::system::error_code
  Socket::listenProto(SocketFactory f,
                      const std::string& host,
                      const std::string& port,
                      BaseFactory bf)
  {
    boost::system::error_code erc;
    typename Proto::endpoint ep = resolve<Proto>(host, port, erc,
                                                 get_io_service());
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
}
