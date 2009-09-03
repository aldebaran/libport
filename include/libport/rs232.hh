/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
#ifndef LIBPORT_RS232_HH
# define LIBPORT_RS232_HH

# include <boost/asio/serial_port.hpp>

namespace libport
{

  struct Rs232: public boost::asio::serial_port
  {
    typedef Rs232 lowest_layer_type;

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

    Rs232(boost::asio::io_service& s);

    void shutdown(int, boost::system::error_code&);
    endpoint remote_endpoint();
    endpoint local_endpoint();
    Rs232& lowest_layer();
    void assign(int, int);

    static const int shutdown_both = 0;
  };

}

# include <libport/rs232.hxx>

#endif // !LIBPORT_RS232_HH
