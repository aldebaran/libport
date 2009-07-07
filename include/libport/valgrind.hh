#ifndef LIBPORT_VALGRIND_HH
# define LIBPORT_VALGRIND_HH

# include <libport/config.h>

# ifdef LIBPORT_HAVE_VALGRIND_VALGRIND_H
#  include <valgrind/valgrind.h>
# else
#  define RUNNING_ON_VALGRIND 0
# endif

#endif
