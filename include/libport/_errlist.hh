/**
 ** \file libport/_errlist.h
 ** \brief Provide a list of Windows error.
 **/

#ifndef LIBPORT__ERRLIST_HH
# define LIBPORT__ERRLIST_HH

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

#endif // !LIBPORT__ERRLIST_HH
