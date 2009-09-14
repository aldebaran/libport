/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
#ifndef LIBPORT_RS232_HXX
# define LIBPORT_RS232_HXX

# include <exception>

namespace libport
{

  inline
  Rs232::Rs232(boost::asio::io_service& s)
    : boost::asio::serial_port(s)
  {
  }

  inline void
  Rs232::shutdown(int, boost::system::error_code&)
  {
    close();
  }

  inline Rs232::endpoint
  Rs232::remote_endpoint()
  {
    throw std::runtime_error
      ("Dummy fonction, remote_endpoint does not make sense in rs232.");
  }

  inline Rs232::endpoint
  Rs232::local_endpoint()
  {
    throw std::runtime_error
      ("Dummy fonction, local_endpoint does not make sense in rs232.");
  }

  inline Rs232&
  Rs232::lowest_layer()
  {
    return *this;
  }

  inline void
  Rs232::assign(int, int)
  {}

}

#endif // !LIBPORT_RS232_HXX
