/// \file libport/version.cc
/// \brief Version of Libport as a package info.

#include <libport/config.h>
#include <libport/package-info.hh>
#include <libport/revision.hh>
#include <libport/version.hh>

namespace libport
{

  const PackageInfo&
  package_info()
  {
    LIBPORT_PACKAGE_INFO_STATIC_VAR_(LIBPORT_PACKAGE_, pi);
    return pi;
  }

}
