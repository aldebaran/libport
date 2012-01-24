/*
 * Copyright (C) 2008-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_UTIME_HH
# define LIBPORT_UTIME_HH

# include <boost/date_time/posix_time/posix_time.hpp>

# include <libport/ctime>
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

  /// From timespec to utime.
  utime_t utime(const timespec& t);

  /// From timeval to utime.
  utime_t utime(const timeval& t);

  /// Return elapsed time from an arbitrary basis in microseconds.
  LIBPORT_API utime_t utime();

  /// Set the arbitrary basis
  LIBPORT_API void utime_reference_set(libport::utime_t ref);

  /// Convert from microseconds to timeval.
  timeval utime_to_timeval(utime_t t);

}

# include <libport/utime.hxx>

#endif // !LIBPORT_UTIME_HH
