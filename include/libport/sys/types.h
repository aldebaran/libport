/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
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

# ifdef _MSC_VER

typedef short mode_t;

typedef int pid_t;

# endif

# ifdef WIN32

// http://www.opengroup.org/onlinepubs/000095399/basedefs/sys/types.h.html
// The type suseconds_t shall be a signed integer type capable of
// storing values at least in the range [-1, 1000000].
typedef long suseconds_t;

# endif

#endif // !LIBPORT_SYS_TYPES_H
