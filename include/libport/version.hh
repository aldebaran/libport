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
