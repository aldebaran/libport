/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
#ifndef LIBPORT_META_VISITOR_HH
# define LIBPORT_META_VISITOR_HH

// Avoid numerous unrelevant warnings triggered by metaprogramming
# if defined __GNUC__
#  pragma GCC system_header
# endif

# include <cstdlib>
# include <iostream>

# include <libport/cassert>
# include <libport/meta.hh>
# include <libport/typelist.hh>
# include <libport/traits.hh>

#define HIERARCHY(L)                            \
  public:                                       \
  typedef L _libport_hierarchy_                 \
                                                \

namespace libport
{
  namespace meta
  {
    class Visitable;

    class BaseVisitor
    {
      public:
        virtual ~BaseVisitor() {}
    };

    template <typename Impl, typename L, typename R, template <class> class Trait>
    class TypeVisitor
      : private typelist::AssertList<L>
    {
      public:
        template <typename T>
        R _libport_visitor_dispatch_(T& elt, int)
        {
          pabort("A visitor failed." << std::endl
                 << "Root node: " << typeid(T).name() << std::endl
                 << "Actual type: " << typeid(elt).name() << std::endl
                 << "Visitor type: " << typeid(*this).name() << std::endl);
	  // This is not enough for Visual C++, which requires an explicit
          // return.  In this case, make a dummy recursive call (which
          // will of course never take place).  We will get a warning,
          // but not an error.
	  return _libport_visitor_dispatch_(elt, 0);
        }
    };

    // FIXME: If the compiler isn't able to tail-unrecurse + inline
    // all the _libport_visitor_dispatch_ functions, this
    // implementation is very expensive: linear wrt the number of
    // visited nodes. We can force an optimized implementation with
    // switches, but this would much heavier, use macros to generate
    // hundred of switches containing hundred of cases, and would
    // limit visitors to an arbitrary size for the visitor hierarchy
    // (ok, we can make it HUGE).  AFAIK, any decent compiler can
    // optimize this, but I might have missed something. Briefly:
    // CHECKME.

    // FIXME: TODO: do not systematically call previsit and
    // postvisit. Make them optional for performances sake.
    template <typename Impl, typename H, typename T, typename R, template <class> class Trait>
    class TypeVisitor<Impl, typelist::List<H, T>, R, Trait>
      : public TypeVisitor<Impl, T, R, Trait>
    {
      public:
        template <typename X>
        R _libport_visitor_dispatch_(X& v, int idx)
        {
          if (idx == 0)
          {
            Impl* impl = static_cast<Impl*>(this);
            impl->preVisit(v);
            R res = impl->visit(static_cast<typename Trait<H>::res>(v));
            impl->postVisit(v);
            return res;
          }
          else
            return TypeVisitor<Impl, T, R, Trait>::_libport_visitor_dispatch_(v, idx - 1);
        }
    };

    template <typename Impl, typename H, typename T, template <class> class Trait>
    class TypeVisitor<Impl, typelist::List<H, T>, void, Trait>
      : public TypeVisitor<Impl, T, void, Trait>
    {
      public:
        template <typename X>
        void _libport_visitor_dispatch_(X& v, int idx)
        {
          if (idx == 0)
          {
            Impl* impl = static_cast<Impl*>(this);
            impl->preVisit(v);
            impl->visit(static_cast<typename Trait<H>::res>(v));
            impl->postVisit(v);
          }
          else
            return TypeVisitor<Impl, T, void, Trait>::_libport_visitor_dispatch_(v, idx - 1);
        }
    };

    template <typename Impl, typename Root, typename R = void, template <class> class Trait = traits::Ref>
    class Visitor:
        private typelist::AssertList<typename Root::_libport_hierarchy_>,
        public TypeVisitor<Impl, typename Root::_libport_hierarchy_, R, Trait>
    {
      public:
        R run (typename Trait<Root>::res e)
        {
          return _libport_visitor_dispatch_(e, meta::deref(e)._libport_visitor_id_());
        }

        R operator() (typename Trait<Root>::res e)
        {
          return run(e);
        }

        virtual ~Visitor() {}

        void preVisit(typename Trait<Root>::res) {}
        void postVisit(typename Trait<Root>::res) {}
    };

    class Visitable
    {
      protected:
        virtual int _libport_visitor_id_() const = 0;
        int _visitable_libport_visitor_id_() const
        {
          return _libport_visitor_id_();
        };
        virtual ~Visitable() {}
        template <typename Impl, typename Root, typename R,
                  template <class> class Trait>
        friend class Visitor;
    };

#define VISITABLE()                                     \
    protected:                                          \
    virtual int _libport_visitor_id_() const            \
    {                                                   \
      return libport::meta::typelist::Idx<              \
      _libport_hierarchy_, _libport_type_>::res;        \
    }                                                   \

  }
}

#endif
