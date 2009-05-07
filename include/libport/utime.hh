#ifndef LIBPORT_UTIME_HH
# define LIBPORT_UTIME_HH

# include <sys/time.h>

# include <libport/export.hh>
# include <libport/sys/time.h>
# include <libport/ufloat.hh>

namespace libport
{
  /// Microseconds.
  typedef long long utime_t;

  /// Convert from seconds to utime_t.
  template <typename Unit>
  utime_t seconds_to_utime(Unit seconds);

  /// From timeval to utime.
  utime_t timeval_to_utime(const timeval& t);

  /// Return elapsed time from an arbitrary basis in microseconds.
  LIBPORT_API utime_t utime();

  /// Convert from microseconds to timeval.
  timeval utime_to_timeval(utime_t t);

}

# include <libport/utime.hxx>

#endif // !LIBPORT_UTIME_HH
