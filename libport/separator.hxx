/**
 ** \file libport/separator.hxx
 ** \brief Output containers with a separator between items.
 */

#ifndef LIBPORT_SEPARATOR_HXX
# define LIBPORT_SEPARATOR_HXX

# include <ostream>
# include "libport/deref.hh"
# include "libport/separator.hh"

namespace libport
{

  template <class C, class S>
  inline
  separator<C, S>::separator (const C& c, const S& s)
    : container_ (c),
      separator_ (s)
  {
  }

  template <class C, typename S>
  inline std::ostream&
  separator<C, S>::operator() (std::ostream& o) const
  {
    typedef typename C::const_iterator iter_type;
    const iter_type begin = container_.begin ();
    const iter_type end   = container_.end ();
    for (iter_type i = begin; i != end; ++i)
      {
	if (i != begin)
	  o << separator_;
	o << deref << *i;
      }
    return o;
  }

  template <class C, typename S>
  separator<C, S>
  separate (const C& c, const S& s)
  {
    return separator<C, S> (c, s);
  }

  template <class C>
  separator<C, char>
  separate (const C& c)
  {
    return separate (c, '\n');
  }

  template <class C, typename S>
  inline std::ostream&
  operator<< (std::ostream& o, const separator<C, S>& s)
  {
    return s (o);
  }

}

#endif // !LIBPORT_SEPARATOR_HXX
