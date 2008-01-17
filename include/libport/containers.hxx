/**
 ** \file libport/containers.hxx
 ** \brief Some syntactic sugar to look for things in STL containers.
 */

#ifndef LIBPORT_CONTAINERS_HXX
# define LIBPORT_CONTAINERS_HXX

# include <ostream>
# include <algorithm>
# include <functional>

# include "libport/containers.hh"

namespace libport
{

  // Object function to free members of a container.
  template<typename T>
  struct Delete : public std::unary_function<T, void>
  {
    void operator () (T t)
    {
      delete t;
    }
  };


  template<typename Container>
  void
  deep_clear (Container& c)
  {
    std::for_each (c.begin (), c.end (),
		   Delete<typename Container::value_type> ());
    c.clear ();
  }


  // Find \a v in the whole \a c.
  template<typename Container>
  inline typename Container::const_iterator
  find (const Container& c, const typename Container::value_type& v)
  {
    return std::find (c.begin (), c.end (), v);
  }

  // Find \a v in the whole \a c.
  template<typename Container>
  inline typename Container::iterator
  find (Container& c, const typename Container::value_type& v)
  {
    return std::find (c.begin (), c.end (), v);
  }


  template<typename Container>
  inline typename Container::mapped_type
  find0 (Container& c, const typename Container::key_type& k)
  {
    typename Container::const_iterator i = c.find(k);
    if (i != c.end())
      return i->second;
    else
      return 0;
  }


  // Apply \a f to all the members of \a c, and return it.
  template<typename Container, typename Functor>
  inline Functor&
  for_each (Container& c, Functor& f)
  {
    for (typename Container::const_iterator i = c.begin (), end = c.end();
	 i != end; ++i)
      f (*i);
    return f;
  }


  // Is \a v member of \a c?
  template<typename Container>
  inline bool
  has (const Container& c, const typename Container::value_type& v)
  {
    // We specify the instance to solve a conflict between the
    // two finds above, that compete against each other because
    // the parameter Container can embed a "const".
    return find<Container> (c, v) != c.end ();
  }


  // Is \a v member of \a c?
  template<typename Container>
  inline bool
  mhas (const Container& c, const typename Container::key_type& v)
  {
    return c.find(v) != c.end ();
  }


  template<typename Map>
  typename Map::iterator
  put (Map& map,
       const typename Map::key_type& key,
       const typename Map::mapped_type& value)
  {
    // See ``Efficient STL''.
    typename Map::iterator i = map.lower_bound (key);

    if (i != map.end () && !(map.key_comp () (key, i->first)))
      {
	// Update.
	i->second = value;
	return i;
      }
    else
      {
	// First insertion.
	typedef typename Map::value_type mvt;
	return map.insert (mvt (key, value)).first;
      }
  }

} // namespace libport

#endif // !LIBPORT_CONTAINERS_HXX
