/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef SERIALIZE_EXPORT_HH
# define SERIALIZE_EXPORT_HH

# include <libport/detect-win32.h>

# ifdef WIN32
#  ifdef BUILDING_SERIALIZE
#   define SERIALIZE_API __declspec(dllexport)
#  else
#   define SERIALIZE_API __declspec(dllimport)
#  endif
# else
#  define SERIALIZE_API __attribute__((visibility("default")))
# endif

#endif // SERIALIZE_EXPORT_HH
