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
 ** \file libport/weak-ptr.hxx
 ** \brief Implementation of libport::weak_ptr.
 **/

#ifndef LIBPORT_WEAK_PTR_HXX
# define LIBPORT_WEAK_PTR_HXX

# include <libport/weak-ptr.hh>

namespace libport
{

  /*-----------------.
  | Ctors and dtor.  |
  `-----------------*/

  template <typename T>
  template <typename U>
  weak_ptr<T>::weak_ptr (const weak_ptr<U>& other)
    : super_type (other)
  {
  }

  template <typename T>
  weak_ptr<T>::weak_ptr (const weak_ptr<T>& other)
    : super_type (other)
  {
  }

  template <typename T>
  weak_ptr<T>::weak_ptr ()
    : super_type ()
  {
  }

  template <typename T>
  weak_ptr<T>::~weak_ptr ()
  {
  }


  /*---------------------.
  | Equality operators.  |
  `---------------------*/

  template <typename T>
  bool
  weak_ptr<T>::operator== (const T* other) const
  {
    return this->get () == other;
  }

  template <typename T>
  bool
  weak_ptr<T>::operator!= (const T* other) const
  {
    return !(*this == other);
  }


  /*--------.
  | Casts.  |
  `--------*/

  template <typename T>
  template <typename U>
  weak_ptr<U>
  weak_ptr<T>::unsafe_cast () const
  {
    weak_ptr<U> res;
    (boost::dynamic_pointer_cast<U, element_type> (*this)).swap (res);
    return res;
  }

  template <typename T>
  template <typename U>
  weak_ptr<U>
  weak_ptr<T>::cast () const
  {
    if (!this->get () || !this->is_a<U> ())
      throw std::bad_cast ();
    return unsafe_cast<U> ();
  }

  template <typename T>
  template <typename U>
  bool
  weak_ptr<T>::is_a () const
  {
    return dynamic_cast<U*> (this->get ());
  }

  /*--------------------------.
  | Free standing functions.  |
  `--------------------------*/

  template <typename T>
  weak_ptr<T>
  make_weak_ptr(T* t)
  {
    return weak_ptr<T> (t);
  }
}

#endif // !LIBPORT_WEAK_PTR_HXX
