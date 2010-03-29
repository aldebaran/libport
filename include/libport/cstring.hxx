/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

namespace libport
{
  inline
  bool
  streq(const char* lhs, const char* rhs)
  {
    return !strcmp(lhs, rhs);
  }

  inline
  bool
  strneq(const char* lhs, const char* rhs, size_t n)
  {
    return !strncmp(lhs, rhs, n);
  }

  // FIXME: might be already present on some systems, or at least
  // optimizable.
  inline
  const char*
  strnchr(const char* str, size_t n, char c)
  {
    for (unsigned i = 0; i < n; ++i)
      if (str[i] == c)
        return str + i;
    return 0;
  }
}
