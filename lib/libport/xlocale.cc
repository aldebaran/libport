/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cassert>
#include <libport/cstdlib>
#include <libport/cerrno>
#include <libport/xlocale.hh>

namespace libport
{
  /*-----------.
  | strto*_c.  |
  `-----------*/

#if LIBPORT_HAVE_XLOCALE_H

  static
  locale_t
  get_c_locale()
  {
    // OS X can be given 0 instead of "C", but GNU Libc then returns 0.
    locale_t res = newlocale(LC_CTYPE_MASK, "C", 0);
    if (!res)
      pabort("cannot create C locale: " << strerror(errno));
    return res;
  }

  static locale_t cloc = assert_exp(get_c_locale());

# define DEFINE(Name, Type)                                     \
  Type                                                          \
  Name ## _c(const char* restrict nptr, char** restrict endptr) \
  {                                                             \
    errno = 0;                                                  \
    return ::Name ## _l(nptr, endptr, cloc);                    \
  }

#else // Typically MSVC.
  static _locale_t cloc = _create_locale(LC_NUMERIC, "C");

# define DEFINE(Name, Type)                                     \
  Type                                                          \
  Name ## _c(const char* restrict nptr, char** restrict endptr) \
  {                                                             \
    errno = 0;                                                  \
    return ::_strtod_l(nptr, endptr, cloc);                     \
  }
#endif

  DEFINE(strtod, double);
  DEFINE(strtof, float);
  DEFINE(strtold, long double);

# undef DEFINE

  /*-------------.
  | strto_c<T>.  |
  `-------------*/
# define DEFINE(Name, Type)                                             \
  template <>                                                           \
  Type                                                                  \
  strto_c<Type>(const char* restrict nptr, char** restrict endptr)      \
  {                                                                     \
    return Name ## _c(nptr, endptr);                                    \
  }

  DEFINE(strtod, double);
  DEFINE(strtof, float);
  DEFINE(strtold, long double);

}
