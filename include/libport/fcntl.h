/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_FNCTL_H
# define LIBPORT_FNCTL_H

# include <fcntl.h>

# include <libport/detect-win32.h>
# include <libport/sys/types.h>

# ifdef WIN32
#  include <io.h>

// Try to give sense to Unix permission flags under WIN32
#  define S_IRWXU _S_IREAD | _S_IWRITE
#  define S_IRUSR _S_IREAD
#  define S_IWUSR _S_IWRITE
#  define S_IXUSR 0
#  define S_IRWXG 0
#  define S_IRGRP 0
#  define S_IWGRP 0
#  define S_IXGRP 0
#  define S_IRWXO 0
#  define S_IROTH 0
#  define S_IWOTH 0
#  define S_IXOTH 0

# endif

// OSX does not have O_LARGEFILE.  No information was found whether
// some equivalent flag is needed or not.  Other projects simply do as
// follows.
# ifndef O_LARGEFILE
#  define O_LARGEFILE 0
# endif

# ifdef WIN32
#  define O_BINARY _O_BINARY
# endif

#endif // !LIBPORT_FNCTL_H
