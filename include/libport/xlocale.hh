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
  LIBPORT_API
  float       strtof_c(const char* restrict nptr, char** restrict end = 0);
  LIBPORT_API
  double      strtod_c(const char* restrict nptr, char** restrict end = 0);
  // Warning: does not work on MSVC, as it uses "double" internally.
  LIBPORT_API
  long double strtold_c(const char* restrict nptr, char** restrict end = 0);


  /*------------------------------------------------------.
  | Template version for float, double, and long double.  |
  `------------------------------------------------------*/

  // No way to please GCC and MSVC with the same code.  See also
  // ufloat.hh.
# if defined _MSC_VER
  template <typename T>
  T
  strto_c(const char* restrict nptr, char** restrict end = 0);

#  define DECLARE(Type)							\
  template <>								\
  LIBPORT_API								\
  Type									\
  strto_c<Type>(const char* restrict nptr, char** restrict end)

  DECLARE(float);
  DECLARE(double);
  DECLARE(long double);
#  undef DECLARE

# else // !defined _MSC_VER
  template <typename T>
  T
  strto_c(const char* restrict nptr, char** restrict end = 0);
# endif
}

#endif // !LIBPORT_LOCALE_HH
