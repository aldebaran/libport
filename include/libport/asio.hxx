/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

namespace libport
{
  namespace netdetail
  {

#if defined LIBPORT_ENABLE_SSL

    struct LIBPORT_API SSLSettings
    {
      boost::asio::ssl::context_base::method context;
      boost::asio::ssl::context::options options;
      std::string privateKeyFile;
      std::string certChainFile;
      std::string tmpDHFile;
      std::string cipherList;
    };
    LIBPORT_API BaseSocket* makeSSLLayer(SSLSettings settings,
                             boost::asio::ip::tcp::socket* s);
#endif
    template<class Stream>
    class SocketImpl;
    class LIBPORT_API SocketImplBase
      : public BaseSocket
      , protected libport::Lockable
    {
    public:
      SocketImplBase()
        : current_(-1)
        , pending_(false)
      {}
      /// Return ammount of data in write buffer
      size_t getWriteBufferContentSize() const;
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
    template<typename T> SocketImplBase* create(T* s)
    {
      return SocketImpl<T>::create(s);
    }
    inline size_t SocketImplBase::getWriteBufferContentSize() const
    {
      return pending_ ? buffers_[1-current_].size() : 0;
    }
  }
  template<class Sock>
  inline void
  Socket::setFD(native_handle_type  fd, typename Sock::protocol_type proto)
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
      (netdetail::SocketImplBase*)netdetail::create(s);
    // (netdetail::SocketImplBase*)netdetail::SocketImpl<Sock>::create(s);
    setBase(b);
    b->startReader();
  }

  inline void
  Socket::syncWrite(const std::string& s)
  {
    syncWrite(s.c_str(), s.length());
  }

  inline void
  Socket::syncWrite(const void* data, size_t length)
  {
    if (base_)
      base_->syncWrite(data, length);
    else
      throw std::runtime_error("Socket is not bound");
  }
}
