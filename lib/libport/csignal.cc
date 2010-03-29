/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cstdio>
#include <libport/csignal>

namespace libport
{
  /// Call std::signal, wrapped with perror on failure.
  sig_t
  signal(int sig, sig_t func)
  {
    sig_t res = ::signal(sig, func);
    if (res == SIG_ERR)
      perror("signal");
    return res;
  }
}
