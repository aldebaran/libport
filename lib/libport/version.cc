/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/version.cc
/// \brief Version of Libport as a package info.

#include <libport/config.h>
#include <libport/package-info.hh>
#include <lib/libport/revision-stub.hh>
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
