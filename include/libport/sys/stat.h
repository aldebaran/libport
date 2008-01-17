#ifndef LIBPORT_SYS_STAT_H_
# define LIBPORT_SYS_STAT_H_

#include <sys/stat.h>

#ifdef WIN32
# ifndef S_ISDIR
#  define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
# endif
#endif

#endif /* !LIBPORT_SYS_STAT_H_ */
