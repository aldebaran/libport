/**
 ** \file libport/shared-ptr.hxx
 ** \brief Implementation of libport::shared_ptr.
 **/

#ifndef LIBPORT_SHARED_REF_HXX
# define LIBPORT_SHARED_REF_HXX

# include "libport/shared-ptr.hh"

namespace libport
{

  /*-----------------.
  | Ctors and dtor.  |
  `-----------------*/

  template <typename T>
  template <typename U>
  shared_ptr<T>::shared_ptr (const shared_ptr<U>& other)
    : super_type (other)
  {
  }

  template <typename T>
  shared_ptr<T>::shared_ptr (const shared_ptr<T>& other)
    : super_type (other)
  {
  }

  template <typename T>
  shared_ptr<T>::shared_ptr (T* p)
    : super_type (p)
  {
  }

  template <typename T>
  shared_ptr<T>::~shared_ptr ()
  {
  }


  /*---------------------.
  | Equality operators.  |
  `---------------------*/

  template <typename T>
  bool
  shared_ptr<T>::operator== (const T* other) const
  {
    return this->get () == other;
  }

  template <typename T>
  bool
  shared_ptr<T>::operator!= (const T* other) const
  {
    return !(*this == other);
  }


  /*--------.
  | Casts.  |
  `--------*/

  template <typename T>
  template <typename U>
  shared_ptr<U>
  shared_ptr<T>::unsafe_cast () const
  {
    shared_ptr<U> res;
    (boost::dynamic_pointer_cast<U, element_type> (*this)).swap (res);
    return res;
  }

  template <typename T>
  template <typename U>
  shared_ptr<U>
  shared_ptr<T>::cast () const
  {
    if (!this->get () || !this->is_a<U> ())
      throw std::bad_cast ();
    return unsafe_cast<U> ();
  }

  template <typename T>
  template <typename U>
  bool
  shared_ptr<T>::is_a () const
  {
    return dynamic_cast<U*> (this->get ());
  }

  /*--------------------------.
  | Free standing functions.  |
  `--------------------------*/

  template <typename T>
  shared_ptr<T>
  make_shared_ptr(T* t)
  {
    return shared_ptr<T> (t);
  }
}

#endif // !LIBPORT_SHARED_REF_HXX
