/**
 ** \file libport/bits/errlist.hh
 ** \brief Provide a list of Windows error.
 **/

#ifndef LIBPORT_BITS_ERRLIST_HH
# define LIBPORT_BITS_ERRLIST_HH

namespace libport
{

# ifdef WIN32

  struct _sys_errlist_t {
    int   code;
    char* msg;
  };

  extern struct _sys_errlist_t _sys_errlist[];

# endif

}

#endif // !LIBPORT_BITS_ERRLIST_HH
