/**
 ** \file libport/stdio.hh
 ** \brief Provide a Windows version of perror.
 **/

#ifndef LIBPORT_STDIO_HH
# define LIBPORT_STDIO_HH

namespace libport
{

  void
  perror(const char* s);

}

#endif // !LIBPORT_STDIO_HH
