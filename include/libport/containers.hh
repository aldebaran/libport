/**
 ** \file libport/containers.hh
 ** \brief Some syntactic sugar to look for things in STL containers.
 */

#ifndef LIBPORT_CONTAINERS_HH
# define LIBPORT_CONTAINERS_HH

# include <deque>
# include <iosfwd>
# include <list>
# include <set>
# include <vector>

namespace libport
{

  /// Invoke delete on all the members of \a c, and clear it.
  template<typename Container>
  void
  deep_clear(Container& c);

  /// Find \a v in the whole \a c.
  template<typename Container>
  inline typename Container::const_iterator
  find(const Container& c, const typename Container::value_type& v);

  /// Find \a v in the whole \a c.
  template<typename Container>
  inline typename Container::iterator
  find(Container& c, const typename Container::value_type& v);

  /// Look up for \c k in \a c, return its value, or 0 if unknown.
  ///
  /// For associative containers mapping pointers.
  template<typename Container>
  inline typename Container::mapped_type
  find0(Container& c, const typename Container::key_type& k);

  /// Apply \a f to all the members of \a c, and return it.
  template<typename Container, typename Functor>
  inline Functor&
  for_each(Container& c, Functor& v);

  /// Is \a v member of \a c?
  template<typename Container>
  inline bool
  has(const Container& c, const typename Container::value_type& v);

  /// Is \a v member of \a c?  Use member find (set, map, hash_map).
  template<typename Container>
  inline bool
  mhas(const Container& c, const typename Container::key_type& v);

  /// Insert or update \a key -> \a value in \a map, return iterator to it.
  template<typename Map>
  typename Map::iterator
  put(Map& map,
      const typename Map::key_type& key,
      const typename Map::mapped_type& value);


  // Does at least one member of \a c satisfy predicate \a f ?
  template<typename Container, typename Functor>
  bool
  has_if(const Container& c, const Functor& f);

  // Erase members of \a c satisfying predicate \a f. This does not
  // preserve the order of the members in the container.
  template<typename Container, typename Functor>
  void
  erase_if(Container& c, const Functor& f);


  // Compare two containers.
  template <template <typename, typename> class Cont, typename E, typename A>
  bool operator==(const Cont<E, A>& lhs, const Cont<E, A>& rhs);
} // namespace libport



namespace std
{

  /*---------------------.
  | Container << Value.  |
  `---------------------*/

#define APPLY_ON_BACK_INSERTION_CONTAINERS(Macro)       \
  Macro(deque);                                         \
  Macro(list);                                          \
  Macro(vector);

  // Push back with '<<'.
#define INSERT(Container)                               \
  template<typename Lhs, typename Alloc,                \
           typename Rhs>                                \
  Container<Lhs, Alloc>&                                \
  operator<<(Container<Lhs, Alloc>& c, const Rhs& v)

  APPLY_ON_BACK_INSERTION_CONTAINERS(INSERT)

#undef INSERT


  /*---------------------------------.
  | Associative Container << Value.  |
  `---------------------------------*/

#define APPLY_ON_ASSOCIATIVE_CONTAINERS(Macro)  \
  Macro(set);

  // Insert with '<<'.
#define INSERT(Container)                               \
  template<typename Lhs, typename Alloc,                \
           typename Rhs>                                \
  Container<Lhs, Alloc>&                                \
  operator<<(Container<Lhs, Alloc>& c, const Rhs& v)

  APPLY_ON_ASSOCIATIVE_CONTAINERS(INSERT)

#undef INSERT


  /*-------------------------.
  | Container << Container.  |
  `-------------------------*/

#define APPLY_ON_CONTAINERS_CONTAINERS(Macro)   \
  Macro(deque, deque);                          \
  Macro(list, list);                            \
  Macro(list, set);                             \
  Macro(set, set);                              \
  Macro(set, vector);                           \
  Macro(vector, set);                           \
  Macro(vector, vector);

  // Concatenate with '<<'.
  // Arguably concatenation should be `+='.  Consider the case
  // of appending a container to a container of containers.  Or use
  // an intermediate function, say 'c1 << contents(c2)'.
#define INSERT(Cont1, Cont2)                    \
  template<typename Lhs, typename Alloc1,       \
           typename Rhs, typename Alloc2>       \
  Cont1<Lhs, Alloc1>&                           \
  operator<< (Cont1<Lhs, Alloc1>& c,            \
              const Cont2<Rhs, Alloc2>& vs)

  APPLY_ON_CONTAINERS_CONTAINERS(INSERT)

#undef INSERT


} // namespace std

# include <libport/containers.hxx>

#endif // !LIBPORT_CONTAINERS_HH
