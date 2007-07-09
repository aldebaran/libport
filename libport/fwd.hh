#ifndef LIBPORT_FWD_HH
# define LIBPORT_FWD_HH

namespace libport
{
  // hash.hh.
  template<class K, class V>
  class hash_map;
  // FIXME: To remove some day.
  template<class K, class V>
  class hash_map_type;

  // lockable.hh.
  class Lockable;

  // ref-pt.hh
  template <class T> class RefPt;

  // select-const.hh.
  template <typename T> struct constify_traits;
  template <typename T> struct id_traits;

  // shared-ref.hh.
  template <typename T> class shared_ref;

  // semaphore.hh.
  class Semaphore;
}

#endif // ! LIBPORT_FWD_HH
