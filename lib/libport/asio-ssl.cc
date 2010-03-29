/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/config.h>
#ifdef LIBPORT_ENABLE_SSL
#include <libport/asio.hh>
#include "asio-impl.hxx"

static const bool ssl_debug=getenv("SSL_DEBUG");
namespace libport
{

  namespace netdetail
  {
    template<typename Stream>
    BaseSocket*
    SSLLayer(SSLSettings settings, Stream* s, bool isServer)
    {
      if (ssl_debug) std::cerr <<"SSLLayer" << std::endl;
      using namespace boost::asio;
      boost::system::error_code erc;
      ssl::context context(s->get_io_service(),
                           isServer?
                           boost::asio::ssl::context::sslv23_server:
                           boost::asio::ssl::context::sslv23_client);
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
      if (ssl_debug) std::cerr <<"handshake " << isServer << std::endl;
      sslStream->handshake(isServer?sslStream->server:sslStream->client, erc);
      if (ssl_debug) std::cerr <<"handshake done: " << erc.message()
         << std::endl;
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
    BaseSocket* makeSSLLayer(SSLSettings settings,
                             boost::asio::ip::tcp::socket* s,
                             bool server)
    {
      return SSLLayer(settings, s, server);
    }
  }

  boost::system::error_code
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
                  settings, _1, true));
    return erc;
  }

  boost::system::error_code
  Socket::connectSSL(const std::string& host, const std::string& port,
                     useconds_t usTimeout,
                     bool asynchronous,
                     boost::asio::ssl::context_base::method ctx,
                     boost::asio::ssl::context::options options,
                     const std::string& privateKeyFile,
                     const std::string& certChainFile,
                     const std::string& tmpDHFile,
                     const std::string& cipherList
                     )
  {
    netdetail::SSLSettings settings;
    settings.context = ctx;
    settings.options = options;
    settings.privateKeyFile = privateKeyFile;
    settings.certChainFile = certChainFile;
    settings.tmpDHFile = tmpDHFile;
    settings.cipherList = cipherList;
    return connectProto<boost::asio::ip::tcp>(host, port, usTimeout, asynchronous,
      (boost::function1<BaseSocket*, boost::asio::ip::tcp::socket*>)
        boost::bind(&netdetail::SSLLayer<boost::asio::ip::tcp::socket>,
                  settings, _1, false));
  }
}
#endif
