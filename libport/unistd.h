#ifndef LIBPORT_UNISTD_H_
# define LIBPORT_UNISTD_H_

# include "detect_win32.h"

# include "libport/config.h"

// This is traditional Unix file.
# ifdef LIBPORT_HAVE_UNISTD_H
#  include "unistd.h"
# endif

// This seems to be its WIN32 equivalent.
// http://msdn2.microsoft.com/en-us/library/ms811896(d=printer).aspx#ucmgch09_topic7.
# if defined WIN32 || defined LIBPORT_WIN32
#  include "io.h"
# endif

#endif /* !LIBPORT_UNISTD_H_ */
