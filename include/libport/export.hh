/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_EXPORT_HH
# define LIBPORT_EXPORT_HH

# include <libport/compiler.hh>

# ifdef BUILDING_LIBPORT
#   define LIBPORT_API ATTRIBUTE_DLLEXPORT
# else
#   define LIBPORT_API ATTRIBUTE_DLLIMPORT
# endif

#endif // LIBPORT_EXPORT_HH
