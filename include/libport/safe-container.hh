/*
 * Copyright (C) 2009-2010, 2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_SAFE_CONTAINER_HH
# define LIBPORT_SAFE_CONTAINER_HH

# include <cstddef> // ptrdiff_t

namespace libport
{
  /** Container wrapper providing iteration robust to removal of any element.
   *
   * To function properly, only a subset of container operations are
   * supported, and incrementing a copy of an iterator different than
   * begin() is not allowed. Furthermore, the number of iterators to
   * not-the-end that can be present at the same time is limited (to at
   * least 32).
   *
   * Usage is simple: Instantiate with container type C (std::list)
   * and element type T. Obtain an iterator by calling begin(). Each
   * call to operator++() on the iterator will give you access to the
   * next element that was present at the time of the begin()
   * call. The call to erase() invalidates *no* iterator (with the
   * exception that you must not try to dereference iterators to
   * removed elements).
   *
   * This implementation is *not* thread-safe.
   *
   * Support for const_iterators is incomplete (we actually return
   * iterators).
   */
  template<template<class U, class V>class C, class T>
  class SafeContainer
  {
  public:
    typedef SafeContainer<C, T> self_type;
    struct real_value_type
    {
      real_value_type(const T &v, self_type& owner);
      unsigned mask;
      T v;
    };
    typedef T value_type;
    typedef T* pointer_type;
    typedef T& reference_type;
    struct Flag
    {
      Flag();
      unsigned long mask;
      unsigned long val;
    };
    typedef C<real_value_type, std::allocator<real_value_type> > container_type_;

    typedef typename container_type_::iterator base_iterator_;

    class iterator
    {
    public:
      typedef SafeContainer<C, T> owner_type;
      typedef T value_type;
      typedef ptrdiff_t difference_type;
      typedef T* pointer_type;
      typedef T* pointer;
      typedef T& reference_type;
      typedef T& reference;
      // Whatever the base container is, we only have forward_iterator support.
      typedef std::forward_iterator_tag iterator_category;

      value_type& operator*();
      value_type* operator->();
      const value_type* operator->() const;
      iterator();
      iterator(const iterator&);
      iterator(const base_iterator_&, owner_type& owner, Flag f);
      ~iterator();
      iterator& operator=(const iterator&);
      bool operator!=(const iterator&)const;
      bool operator==(const iterator&)const;
      void operator++();
      void operator++(int);
    private:
      void destroy();
      void next();
      friend class SafeContainer<C, T>;
      Flag flag;
      SafeContainer<C, T>* owner;
      base_iterator_ base;
    };
    typedef iterator const_iterator;

    SafeContainer();
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    void erase(iterator v);
    void push_back(const T& t);
    void push_front(const T& t);
    void pop_front();
    void pop_back();
    void clear();
    bool empty() const;
    size_t size() const;
    template<typename I>
    void insert(iterator where, I beg, I end);
  private:
    /* How it works: each element has a bitmask to keep track of whether each
     * iterator passed through it. Each iterator is associated with a bit
     * number, and transition direction (from 0 to 1 or 1 to 0).
     * getFlag() is used to retreive an unused bit number. Transition direction
     * is reversed each time the same bit is reused.
     * When iterator++ is called, the iterator looks for the next element with
     * its bit in the 'before transition' state, switches it and returns it.
     * Any call to erase() marks all iterators as dirty. When this happens,
     * next call to iterator++ will look for the first element with its
     * bit in the 'before transition' state, starting from begin().
     * New elements are inserted with all mask bits in the 'after transition'
     * value, so that they are ignored by currently existing iterators.
     * Destruction of an iterator will force it to iterate to the end, so
     * that no initialization is required when another iterator will reuse
     * the same bit.
     * The iterator mark his bit as unused as soon as it reaches end.
     * Making a copy of an iterator equals to begin() will give the copy a
     * new bit number. Making a copy of an iterator being neither begin() nor
     * end() will create an iterator on which ++ cannot be called.
     */
    /// Get next available flag.
    Flag getFlag();
    // bit x=1 means that flag x is making the bit go from 1 to 0
    unsigned long currentMask;
    unsigned long invalidationMask;
    // Mask of bits currently in use.
    unsigned long inUse;
    friend class iterator;
    container_type_ container;
  };
}

# include "libport/safe-container.hxx"

#endif
