/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_SYS_STAT_H
# define LIBPORT_SYS_STAT_H

# include <sys/stat.h>
# include <libport/sys/types.h>
# include <libport/detect-win32.h>

# ifdef WIN32

#  ifndef S_ISDIR
#   define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
#  endif

int mkdir(const char *path, mode_t mode);

# endif

# include <libport/sys/stat.hxx>

#endif // !LIBPORT_SYS_STAT_H
