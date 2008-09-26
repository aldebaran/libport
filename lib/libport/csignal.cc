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
