/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef SCHED_EXPORT_HH
# define SCHED_EXPORT_HH

# include <libport/compiler.hh>

# ifdef BUILDING_LIBSCHED
#   define SCHED_API ATTRIBUTE_DLLEXPORT
# else
#   define SCHED_API ATTRIBUTE_DLLIMPORT
# endif

#endif // SCHED_EXPORT_HH
