#ifndef LIBPORT_FWD_HH
# define LIBPORT_FWD_HH

namespace libport
{
  // hash.hh.
  template<class K, class V>
  class hash_map_type;

  // lockable.hh.
  class Lockable;

  // ref-pt.hh
  template <class T> class RefPt;

  // semaphore.hh.
  class Semaphore;
}

#endif // ! LIBPORT_FWD_HH
