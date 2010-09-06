/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** \file libport/xalloc.hxx
 ** \brief Implementation for libport/xalloc.hh.
 */

#ifndef LIBPORT_XALLOC_HXX
# define LIBPORT_XALLOC_HXX

# include "libport/xalloc.hh"

namespace libport
{

  /*----------------.
  | iomanipulator.  |
  `----------------*/

  inline std::ostream&
  operator<<(std::ostream& o, const iomanipulator& g)
  {
    g(o);
    return o;
  }


  /*---------------------.
  | xalloc<StoredType>.  |
  `---------------------*/

  template <class StoredType>
  xalloc<StoredType>::xalloc()
    : index_(std::ios::xalloc())
  {
  }

  template <class StoredType>
  long int
  xalloc<StoredType>::index() const
  {
    return index_;
  }

  template <class StoredType>
  StoredType&
  xalloc<StoredType>::operator()(std::ostream& ostr) const
  {
    // FIXME: Some meta programming to switch on pword or iword
    // would make it possible to use a regular static_cast instead
    // of this C-cast.
    return (StoredType&) (ostr.pword(index()));
  }


  /*-----------.
  | set_type.  |
  `-----------*/

  template <class StoredType>
  xalloc<StoredType>::set_type::set_type(const xalloc& slot, StoredType& data) :
    slot_(slot), data_(data)
  {
  }

  template <class StoredType>
  void
  xalloc<StoredType>::set_type::operator()(std::ostream& ostr) const
  {
    slot_(ostr) = data_;
  }

  template <class StoredType>
  typename xalloc<StoredType>::set_type
  xalloc<StoredType>::set(StoredType& data) const
  {
    return set_type(*this, data);
  }


  /*-----------.
  | get_type.  |
  `-----------*/

  template <class StoredType>
  xalloc<StoredType>::get_type::get_type(const xalloc& slot, StoredType& data)
    : slot_(slot)
    , data_(data)
  {
  }

  template <class StoredType>
  void
  xalloc<StoredType>::get_type::operator()(std::ostream& ostr) const
  {
    data_ = slot_(ostr);
  }

  template <class StoredType>
  typename xalloc<StoredType>::get_type
  xalloc<StoredType>::get(StoredType& data) const
  {
    return get_type(*this, data);
  }


  /*------------.
  | swap_type.  |
  `------------*/

  template <class StoredType>
  xalloc<StoredType>::swap_type::swap_type(const xalloc& slot, StoredType& data)
    : slot_(slot)
    , data_(data)
  {
  }

  template <class StoredType>
  void
  xalloc<StoredType>::swap_type::operator()(std::ostream& ostr) const
  {
    std::swap(slot_(ostr), data_);
  }

  template <class StoredType>
  typename xalloc<StoredType>::swap_type
  xalloc<StoredType>::swap(StoredType& data) const
  {
    return swap_type(*this, data);
  }

}

#endif // !LIBPORT_XALLOC_HXX
