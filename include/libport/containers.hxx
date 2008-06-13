/**
 ** \file libport/containers.hxx
 ** \brief Some syntactic sugar to look for things in STL containers.
 */

#ifndef LIBPORT_CONTAINERS_HXX
# define LIBPORT_CONTAINERS_HXX

# include <algorithm>
# include <functional>
# include <ostream>

# include <boost/range.hpp>

# include <libport/containers.hh>
# include <libport/foreach.hh>

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


  // Find \a v in the whole \a c.
  template<typename Container>
  inline typename Container::const_iterator
  find(const Container& c, const typename Container::value_type& v)
  {
    return std::find(begin(c), end(c), v);
  }

  // Find \a v in the whole \a c.
  template<typename Container>
  inline typename Container::iterator
  find(Container& c, const typename Container::value_type& v)
  {
    return std::find(begin(c), end(c), v);
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

#endif // !LIBPORT_CONTAINERS_HXX
