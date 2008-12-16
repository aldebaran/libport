#ifndef LIBPORT_FNCTL_H
# define LIBPORT_FNCTL_H

# include <fcntl.h>

// OSX does not have O_LARGEFILE.  No information was found whether
// some equivalent flag is needed or not.  Other projects simply do as
// follows.
# ifndef O_LARGEFILE
#  define O_LARGEFILE 0
# endif

#endif // !LIBPORT_FNCTL_H
