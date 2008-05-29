#ifndef LIBPORT_SYS_PARAM_H
# define LIBPORT_SYS_PARAM_H

# ifdef LIBPORT_HAVE_SYS_PARAM_H
#  include <sys/param.h>
# endif

# ifndef MAXPATHLEN
#  define MAXPATHLEN 4096
# endif

#endif
