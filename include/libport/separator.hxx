/**
 ** \file libport/separator.hxx
 ** \brief Output containers with a separator between items.
 */

#ifndef LIBPORT_SEPARATOR_HXX
# define LIBPORT_SEPARATOR_HXX

# include <ostream>

# include <libport/deref.hh>
# include <libport/foreach.hh>
# include <libport/separator.hh>

namespace libport
{

  template <typename C, typename S>
  inline
  separator<C, S>::separator (const C& c, const S& s)
    : container_ (c),
      separator_ (s)
  {
  }

  template <typename C, typename S>
  inline std::ostream&
  separator<C, S>::operator() (std::ostream& o) const
  {
    bool tail = false;
    foreach (typename C::const_reference e, container_)
    {
      if (tail++)
        o << separator_;
      o << deref << e;
    }
    return o;
  }

  template <typename C, typename S>
  separator<C, S>
  separate (const C& c, const S& s)
  {
    return separator<C, S> (c, s);
  }

#if 0
  // Checking whether it makes it easier for some compilers:
  /*
    ast/nary.cc(107) : error C2780: 'libport::separator<C,char> libport::separate(const C &)' : expects 1 arguments - 2 provided

    C:/cygwin/home/build/kernel2/kernel2_winxp_vcxx/build/libport/separator.hh(33) : see declaration of 'libport::separate'

    ast/nary.cc(107) : error C2784: 'libport::separator<C,S> libport::separate(const C &,const S &)' : could not deduce template argument for 'overloaded function type' from 'overloaded function type'

    C:/cygwin/home/build/kernel2/kernel2_winxp_vcxx/build/libport/separator.hh(28) : see declaration of 'libport::separate'

    ast/nary.cc(107) : error C2784: 'libport::separator<C,S> libport::separate(const C &,const S &)' : could not deduce template argument for 'const C &' from 'const ast::exec_exps_type'

    C:/cygwin/home/build/kernel2/kernel2_winxp_vcxx/build/libport/separator.hh(28) : see declaration of 'libport::separate'

  */

  /*
    ast/nary.cc: In function `std::ostream& ast::operator<<(std::ostream&, const ast::exec_exps_type&)':
    ast/nary.cc:107: error: no matching function for call to `separate(const std::list<ast::exec_exp_type, std::allocator<ast::exec_exp_type> >&, std::ostream&(&)(std::ostream&))'
   make[3]: *** [nary.lo] Error 1
  */
    template <typename C>
  separator<C, char>
  separate (const C& c)
  {
    return separate (c, '\n');
  }
#endif
  template <typename C, typename S>
  inline std::ostream&
  operator<< (std::ostream& o, const separator<C, S>& s)
  {
    return s(o);
  }

}

#endif // !LIBPORT_SEPARATOR_HXX
