/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/cmath.hxx
/// \brief Portable <cmath>

# include <libport/config.h>

namespace libport
{

  /*--------.
  | round.  |
  `--------*/

# if defined LIBPORT_HAVE_ROUND
#  define ROUND_IF(True, False) True
# else
#  define ROUND_IF(True, False) False
# endif

# define ROUND(Type)                                            \
  inline Type round(Type v)                                     \
  {                                                             \
    return                                                      \
      ROUND_IF(::round(v),                                      \
               (v < 0. ? -1. : 1.0)                             \
               * std::floor(std::fabs(v) + 0.5));               \
  }
  ROUND(float);
  ROUND(double);
  ROUND(long double);

# undef ROUND
# undef ROUND_IF



  /*--------.
  | trunc.  |
  `--------*/

# if defined LIBPORT_HAVE_TRUNC
#  define TRUNC_IF(True, False) True
# else
#  define TRUNC_IF(True, False) False
# endif

# define TRUNC(Type)                                            \
  inline Type trunc(Type v)                                     \
  {                                                             \
    return                                                      \
      TRUNC_IF(::trunc(v),                                      \
               v < 0. ? ceil(v) : floor(v));                    \
  }
  TRUNC(float);
  TRUNC(double);
  TRUNC(long double);

# undef TRUNC
# undef TRUNC_IF

}
