#ifndef LIBPORT_FWD_HH
# define LIBPORT_FWD_HH

namespace libport
{
  // hash.hh.
  template<class K, class V>
  class hash_map_type;

  // ref-pt.hh
  template <class T> class RefPt;
}

namespace urbi
{
  // lockable.hh.
  class Lockable;

  // semaphore.hh.
  class Semaphore;
}
#endif // ! LIBPORT_FWD_HH
