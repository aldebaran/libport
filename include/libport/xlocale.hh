/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_XLOCALE_HH
# define LIBPORT_XLOCALE_HH

# include <libport/config.h>
# include <libport/compiler.hh>
# include <libport/export.hh>
# if LIBPORT_HAVE_XLOCALE_H
#  include <xlocale.h>
# endif

namespace libport
{
  /// Wrapper around std::strto*_l with the C locale.
  /// Sets errno to 0 beforehand.
  float       strtof_c(const char* restrict nptr, char** restrict end);
  double      strtod_c(const char* restrict nptr, char** restrict end);
  long double strtold_c(const char* restrict nptr, char** restrict end);

  // Template version for float, double, and long double.
  template <typename T>
  T
  strto_c(const char* restrict nptr, char** restrict end);
}

#endif // !LIBPORT_LOCALE_HH
