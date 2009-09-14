/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_SYS_TYPES_H
# define LIBPORT_SYS_TYPES_H

# include <sys/types.h>
# include <libport/detect-win32.h>

# ifdef WIN32

typedef short mode_t;

# endif

#endif // !LIBPORT_SYS_TYPES_H
