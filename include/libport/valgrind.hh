#ifndef LIBPORT_VALGRIND_HH
# define LIBPORT_VALGRIND_HH

# ifdef HAVE_VALGRIND_VALGRIND_HH
#  include <valgrind/valgrind.hh>
# else
#  define RUNNING_ON_VALGRIND 0
# endif

#endif
