#ifndef LIBPORT_UTIME_HH
# define LIBPORT_UTIME_HH

# include <sys/time.h>

# include <libport/export.hh>
# include <libport/sys/time.h>

namespace libport
{
  /// Microseconds.
  typedef long long utime_t;

  /// Convert from microseconds to timeval.
  struct timeval utime_to_timeval(utime_t t);

  /// Return elapsed time from an arbitrary basis in microseconds.
  LIBPORT_API utime_t utime();
}

# include <libport/utime.hxx>

#endif // !LIBPORT_UTIME_HH
