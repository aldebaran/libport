/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/cast.hxx
/// \brief Libport.Cast.

template <typename In, typename Out>
ATTRIBUTE_ALWAYS_INLINE
ATTRIBUTE_UNUSED_RESULT
Out
union_cast(In in)
{
  union
  {
    In in;
    Out out;
  } cast;
  cast.in = in;
  return cast.out;
}
