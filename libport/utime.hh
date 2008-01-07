#ifndef LIBPORT_UTIME_HH
# define LIBPORT_UTIME_HH


namespace libport
{
  typedef long long utime_t;
  /// Return elapsed time from an arbitrary basis in microseconds.
  utime_t utime();
}


#endif // !LIBPORT_UTIME_HH
