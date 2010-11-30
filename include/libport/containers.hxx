/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** \file libport/containers.hxx
 ** \brief Some syntactic sugar to look for things in STL containers.
 */

#ifndef LIBPORT_CONTAINERS_HXX
# define LIBPORT_CONTAINERS_HXX

# include <algorithm>
# include <functional>
# include <set>
# include <ostream>

# include <boost/range.hpp>
# include <boost/unordered_set.hpp>

# include <libport/containers.hh>
# include <libport/foreach.hh>
# include <libport/preproc.hh>

namespace libport
{
  using boost::begin;
  using boost::end;

  template<typename Container>
  void
  deep_clear(Container& c)
  {
    foreach(const typename Container::value_type& v, c)
      delete v;
    c.clear();
  }

  template <typename Container>
  struct Finder
  {
    typedef Container container;
    typedef typename container::iterator iterator;
    typedef typename container::value_type value_type;
    static iterator
    find(container& c, const value_type& v)
    {
      return std::find(begin(c), end(c), v);
    }
  };

#define LIBPORT_CONTAINER_FIND_MEMBER(Template, Type)   \
  template<LIBPORT_UNWRAP(Template)>                    \
  struct Finder<LIBPORT_UNWRAP(Type)>                   \
  {                                                     \
    typedef LIBPORT_UNWRAP(Type) container;             \
    typedef typename container::iterator iterator;      \
    typedef typename container::value_type value_type;  \
    static iterator                                     \
    find(container& c, const value_type& v)             \
    {                                                   \
      return c.find(v);                                 \
    }                                                   \
  };                                                    \

  LIBPORT_CONTAINER_FIND_MEMBER(
    (typename Value, typename Hash, typename Pred, typename Alloc),
    (boost::unordered_set<Value, Hash, Pred, Alloc>));
  LIBPORT_CONTAINER_FIND_MEMBER(
    (typename Key, typename Compare, typename Alloc),
    (std::set<Key, Compare, Alloc>));

#undef LIBPORT_CONTAINER_FIND_MEMBER

  // Find \a v in the whole \a c.
  template<typename Container>
  inline typename Container::const_iterator
  find(const Container& c, const typename Container::value_type& v)
  {
    return find(const_cast<Container&>(c), v);
  }

  // Find \a v in the whole \a c.
  template<typename Container>
  inline typename Container::iterator
  find(Container& c, const typename Container::value_type& v)
  {
    return Finder<Container>::find(c, v);
  }

  template<typename Container>
  inline typename Container::mapped_type
  find0(Container& c, const typename Container::key_type& k)
  {
    typename Container::const_iterator i = c.find(k);
    if (i != end(c))
      return i->second;
    else
      return 0;
  }


  // Apply \a f to all the members of \a c, and return it.
  template<typename Container, typename Functor>
  inline Functor&
  for_each(Container& c, Functor& f)
  {
    foreach(const typename Container::value_type& i, c)
      f(c);
    return f;
  }


  // Is \a v member of \a c?
  template<typename Container>
  inline bool
  has(const Container& c, const typename Container::value_type& v)
  {
    // We specify the instance to solve a conflict between the
    // two finds above, that compete against each other because
    // the parameter Container can embed a "const".
    return find<Container>(c, v) != end(c);
  }


  // Is \a v member of \a c?
  template<typename Container>
  inline bool
  mhas(const Container& c, const typename Container::key_type& v)
  {
    return c.find(v) != end(c);
  }


  template<typename Map>
  typename Map::iterator
  put(Map& map,
      const typename Map::key_type& key,
      const typename Map::mapped_type& value)
  {
    // See ``Efficient STL''.
    typename Map::iterator i = map.lower_bound(key);

    if (i != end(map) && !map.key_comp()(key, i->first))
      {
	// Update.
	i->second = value;
	return i;
      }
    else
      {
	// First insertion.
	typedef typename Map::value_type mvt;
	return map.insert(mvt(key, value)).first;
      }
  }

  template<typename Container, typename Functor>
  inline bool
  has_if(const Container& c, const Functor& f)
  {
    typename Container::const_iterator end_c = end(c);
    return std::find_if(begin(c), end_c, f) != end_c;
  }

  template<typename Container, typename Functor>
  inline void
  erase_if(Container& c, const Functor& f)
  {
    typename Container::iterator end_c = end(c);
    c.erase(std::remove_if(begin(c), end_c, f), end_c);
  }

} // namespace libport

namespace std
{

  /*---------------------.
  | Container << Value.  |
  `---------------------*/

#define INSERT2(Container, Function)                    \
  template<typename Lhs, typename Alloc,                \
           typename Rhs>                                \
  Container<Lhs, Alloc>&                                \
  operator<<(Container<Lhs, Alloc>& c, const Rhs& v)    \
  {                                                     \
    c.Function(v);                                      \
    return c;                                           \
  }

#define INSERT(Container) INSERT2(Container, push_back)
  APPLY_ON_BACK_INSERTION_CONTAINERS(INSERT)
#undef INSERT

#define INSERT(Container) INSERT2(Container, insert)
  APPLY_ON_ASSOCIATIVE_CONTAINERS(INSERT)
#undef INSERT
#undef INSERT2


  /*-------------------------.
  | Container << Container.  |
  `-------------------------*/

#define INSERT(Cont1, Cont2)                    \
  template<typename Lhs, typename Alloc1,       \
           typename Rhs, typename Alloc2>       \
  Cont1<Lhs, Alloc1>&                           \
  operator<<(Cont1<Lhs, Alloc1>& c,             \
             const Cont2<Rhs, Alloc2>& vs)      \
  {                                             \
    c.insert(c.end(), vs.begin(), vs.end());    \
    return c;                                   \
  }

  INSERT(::std::deque,  ::std::deque);
  INSERT(::std::list,   ::std::list);
  INSERT(::std::list,   ::std::set);
  INSERT(::std::vector, ::std::set);
  INSERT(::std::vector, ::std::vector);

#undef INSERT



  /*-------------------.
  | set << Container.  |
  `-------------------*/

#define INSERT(Cont1, Cont2)                    \
  template<typename Lhs, typename Alloc1,       \
           typename Rhs, typename Alloc2>       \
  Cont1<Lhs, Alloc1>&                           \
  operator<<(Cont1<Lhs, Alloc1>& c,             \
             const Cont2<Rhs, Alloc2>& vs)      \
  {                                             \
    foreach (const Rhs& v, vs)                  \
      c << v;                                   \
    return c;                                   \
  }

  INSERT(::std::set, ::std::set);
  INSERT(::std::set, ::std::vector);

#undef INSERT

  template <typename T, typename Alloc>
  inline
  std::ostream&
  operator<< (std::ostream& out, const std::vector<T, Alloc>& c)
  {
    out << "[";
    typedef typename std::vector<T, Alloc>::value_type value_type;
    foreach (const value_type& v, c)
      out << v << ", ";
    out << "]";
    return out;
  }

  template <typename K, typename V, typename Alloc>
  std::ostream&
  operator<< (std::ostream& out, const boost::unordered_map<K, V, Alloc>& c)
  {
    out << "[";
    typedef typename boost::unordered_map<K, V, Alloc>::value_type value_type;
    foreach (const value_type& v, c)
      out << v.first << ": " << v.second << ", ";
    out << "]";
    return out;
  }

  template <typename F, typename S>
  std::ostream&
  operator<< (std::ostream& out, const std::pair<F, S>& c)
  {
    out << "(" << c.first << ", " << c.second << ")";
    return out;
  }

} // namespace std

#endif // !LIBPORT_CONTAINERS_HXX
