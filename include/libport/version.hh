/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/version.hh
/// \brief Version of Libport as a package info.

#ifndef LIBPORT_VERSION_HH
# define LIBPORT_VERSION_HH

# include <libport/fwd.hh>
# include <libport/export.hh>

namespace libport
{
  LIBPORT_API const PackageInfo& package_info();
}

#endif // !LIBPORT_VERSION_HH
