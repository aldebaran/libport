/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_SAFE_CONTAINER_HXX
# define LIBPORT_SAFE_CONTAINER_HXX

# include <stdexcept>
# include <iostream>

# include <libport/foreach.hh>

namespace libport
{

  template<template<class, class>class C, class T>
  inline
  SafeContainer<C, T>::SafeContainer()
   : currentMask(0)
   , invalidationMask(0)
   , inUse(0)
  {
  }

#define CONTAINER                               \
  SafeContainer<C, T>

#define CONTAINER_SMETHOD(Ret)                          \
  template<template<class, class>class C, class T>      \
  inline						\
  Ret                                                   \
  CONTAINER::

#define CONTAINER_METHOD(Ret)                           \
  CONTAINER_SMETHOD(typename CONTAINER::Ret)

  CONTAINER_METHOD(iterator::value_type&) iterator::operator*()
  {
    return base->v;
  }

  CONTAINER_METHOD(iterator::value_type*) iterator::operator->()
  {
    return & base->v;
  }

  template<template<class, class>class C, class T>
  inline
  const typename SafeContainer<C, T>::iterator::value_type*
  SafeContainer<C, T>::iterator::operator->() const
  {
    return &base->v;
  }

  CONTAINER_SMETHOD(__)
  real_value_type::real_value_type(const value_type &v,
                                   self_type& owner)
    : v(v)
  {
    mask = owner.currentMask;
  }

  CONTAINER_SMETHOD(__)
  Flag::Flag()
    : mask(0)
    , val(0)
  {}

  CONTAINER_SMETHOD(__)
  iterator::iterator()
    : owner(0)
  {
  }

  CONTAINER_SMETHOD(__)
  iterator::iterator(const iterator& b)
    : owner(b.owner)
    , base(b.base)
  {
    *this = b;
  }

  CONTAINER_SMETHOD(__)
  iterator::iterator(const base_iterator_& b,
                     CONTAINER& owner, Flag f)
    : flag(f)
    , owner(&owner)
    , base(b)
  {
  }


  CONTAINER_SMETHOD(__)
  iterator::~iterator()
  {
    destroy();
  }

  CONTAINER_SMETHOD(void) iterator::destroy()
  {
    // We must iterate until the end to fix flags
    while (flag.mask)
      next();
  }

  CONTAINER_METHOD(iterator&) iterator::operator=(const iterator& b)
  {
    destroy();
    if (!b.owner)
      return *this;
    base = b.base;
    owner = b.owner;
    flag = b.flag;
    const base_iterator_& bi = b.base;
    if (bi == owner->container.begin() && bi != owner->container.end())
    {
      flag = owner->getFlag();
      bi->mask ^= flag.mask;
    }
    else
    {
      if (flag.mask)
      {
        // Get us a new flag, and advance to be at the same position as b.
        // It would not be safe to use b.base_iterator_::operator== as it
        // might have been invalidated.
        flag = owner->getFlag();
        foreach (real_value_type& v, owner->container)
          if ((v.mask & b.flag.mask) != b.flag.val)
            break;
          else
            v.mask ^= flag.mask;
      }
    }
    return *this;
  }

  CONTAINER_SMETHOD(bool) iterator::operator != (const iterator& b) const
  {
    return ! ((*this) == b);
  }

  CONTAINER_SMETHOD(bool) iterator::operator == (const iterator& b) const
  {
    if (!flag.mask || !b.flag.mask)
      return (!flag.mask && !b.flag.mask);
    // Do not assume the parent operator is a method and write
    // something like base_iterator_::operator ==.
    return base == b.base;
  }

  CONTAINER_SMETHOD(void) iterator::next()
  {
    if (!flag.mask)
      return; // already at end
    if (owner->invalidationMask & flag.mask)
    {
      base_iterator_ i;
      //Find first non-marked element
      for (i = owner->container.begin(); i != owner->container.end(); ++i)
      {
        if ((i->mask & flag.mask) != flag.val)
          break;
      }
      base = i;
      owner->invalidationMask &= ~flag.mask;
    }
    else
      /* We must also perform a check if no invalidation to ignore newly
       * added elements.
       */
      do {
        base++;
      } while  (base != owner->container.end()
                 && (flag.mask & base->mask) == flag.val);
    if (base == owner->container.end())
    {
      owner->inUse &= ~flag.mask;
      flag.val = flag.mask = 0;
    }
    else
    {
      base->mask ^= flag.mask;
    }
  }

  //pre-increment
  CONTAINER_SMETHOD(void) iterator::operator ++()
  {
    next();
  }

  //post-increment
  CONTAINER_SMETHOD(void) iterator::operator ++(int)
  {
    next();
  }

  CONTAINER_METHOD(iterator) begin()
  {
    if (container.empty())
      return end();
    Flag f = getFlag();
    //mark front
    container.front().mask ^= f.mask;
    return iterator(container.begin(), *this, f);
  }

  CONTAINER_METHOD(iterator) end()
  {
    return iterator(container.end(), *this, Flag());
  }

  CONTAINER_METHOD(iterator) begin() const
  {
    return const_cast<self_type*>(this)->begin();
  }

  CONTAINER_METHOD(iterator) end() const
  {
    return const_cast<self_type*>(this)->end();
  }

  CONTAINER_SMETHOD(void) erase(iterator v)
  {
    invalidationMask = -1;
    container.erase(v.base);
  }

  CONTAINER_SMETHOD(void) push_back(const value_type& t)
  {
    container.push_back(real_value_type(t, *this));
  }

  CONTAINER_SMETHOD(void) push_front(const value_type& t)
  {
    container.push_front(real_value_type(t, *this));
  }

  CONTAINER_SMETHOD(void) pop_front()
  {
    container.pop_front();
    invalidationMask = -1;
  }

  CONTAINER_SMETHOD(void) pop_back()
  {
    container.pop_back();
    invalidationMask = -1;
  }

  CONTAINER_SMETHOD(void) clear()
  {
    container.clear();
    invalidationMask = -1;
  }

  CONTAINER_SMETHOD(size_t) size() const
  {
    return container.size();
  }

  CONTAINER_SMETHOD(bool) empty() const
  {
    return container.empty();
  }

  template<template<class, class>class C, class T>
  template<typename I>
  inline
  void CONTAINER::insert(iterator where, I beg, I end)
  {
    base_iterator_ l = where.base;
    for (I i = beg; i != end; ++i)
    {
      l = container.insert(l, real_value_type(*i, *this));
      l++;
    }
  }

  CONTAINER_METHOD(Flag) getFlag()
  {
    unsigned nextFlag;
    for (nextFlag = 1; nextFlag && (inUse & nextFlag); nextFlag<<=1)
      ;
    if (!nextFlag)
      throw std::runtime_error("Too many iterators on SafeContainer");
    currentMask ^= nextFlag;

    // Reset invalidation mask to 0 for this flag
    inUse |= nextFlag;
    invalidationMask &= ~nextFlag;
    Flag res;
    res.mask = nextFlag;
    res.val = currentMask & nextFlag;
    return res;
  }
}
#undef CONTAINER
#undef CONTAINER_METHOD
#undef CONTAINER_SMETHOD

#endif
