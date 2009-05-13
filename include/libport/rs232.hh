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
