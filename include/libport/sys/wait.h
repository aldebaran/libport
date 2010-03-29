/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_SYS_WAIT_H
# define LIBPORT_SYS_WAIT_H

# include <libport/config.h>

# if LIBPORT_HAVE_SYS_WAIT_H
#  include <sys/wait.h>
# endif

# include <libport/detect-win32.h>

# if defined WIN32

// Under windows, consider programs systematically exit.
#  define WIFEXITED(Status) true
#  define WEXITSTATUS(Status) Status

# endif

#endif // !LIBPORT_SYS_WAIT_H
