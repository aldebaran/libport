/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cmath>

namespace libport
{
  template <typename T>
  inline
  T
  between(const T& min, const T& value, const T& max)
  {
    if (value < min)
      return min;
    else if (max < value)
      return max;
    else
      return value;
  }

  template <typename T>
  inline
  T
  rad2deg(const T& value)
  {
    return value * (180.0 / M_PI);
  }

  template <typename T>
  inline
  T
  deg2rad(const T& value)
  {
    return value * (M_PI / 180.0);
  }

}

