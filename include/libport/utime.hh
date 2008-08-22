#ifndef LIBPORT_UTIME_HH
# define LIBPORT_UTIME_HH

# include <libport/export.hh>
namespace libport
{
  typedef long long utime_t;
  /// Return elapsed time from an arbitrary basis in microseconds.
  LIBPORT_API utime_t utime();
}


#endif // !LIBPORT_UTIME_HH
