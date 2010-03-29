/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_SYS_PARAM_H
# define LIBPORT_SYS_PARAM_H

# ifdef LIBPORT_HAVE_SYS_PARAM_H
#  include <sys/param.h>
# endif

# ifndef MAXPATHLEN
#  define MAXPATHLEN 4096
# endif

#endif
