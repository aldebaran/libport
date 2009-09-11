/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

namespace libport
{
  template <typename T>
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

}

