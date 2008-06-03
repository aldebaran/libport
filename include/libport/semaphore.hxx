#ifndef LIBPORT_SEMAPHORE_HXX
# define LIBPORT_SEMAPHORE_HXX

# include <libport/semaphore.hh>

namespace libport
{

  inline void
  Semaphore::operator++ (int)
  {
    Semaphore::operator++ ();
  }

  inline void
  Semaphore::operator-- (int)
  {
    Semaphore::operator-- ();
  }

} // namespace libport

#endif // !LIBPORT_SEMAPHORE_HXX
