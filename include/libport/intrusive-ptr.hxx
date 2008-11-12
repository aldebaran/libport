/**
 ** \file libport/intrusive-ptr.hxx
 ** \brief Implementation of libport::intrusive_ptr.
 **/

#ifndef LIBPORT_BOOST_INTRUSIVE_PTR_HXX
# define LIBPORT_BOOST_INTRUSIVE_PTR_HXX

# include <cassert>
# include <typeinfo>

namespace libport
{
  template <typename T>
  template <typename U>
  intrusive_ptr<T>::intrusive_ptr(const intrusive_ptr<U>& other)
    :pointee_(0)
  {
    (*this) = other;
  }
  template <typename T>
  intrusive_ptr<T>::intrusive_ptr(const intrusive_ptr<T>& other)
    :pointee_(0)
  {
    (*this) = other;
  }
  template <typename T>
  intrusive_ptr<T>::intrusive_ptr (T* p)
  :pointee_(0)
  {
    (*this) = p;
  }

  template <typename T>
  intrusive_ptr<T>::~intrusive_ptr()
  {
    // This cast is required, or the compiler uses the intrusive_ptr ctor,
    // leading to an infinite loop.
    (*this) = (T*)0;
  }

  template <typename T>
  template <typename U>
  intrusive_ptr<T>&
  intrusive_ptr<T>::operator = (const intrusive_ptr<U>& other)
  {
    return (*this) = other.get();
  }

  template <typename T>
  intrusive_ptr<T>&
  intrusive_ptr<T>::operator = (const intrusive_ptr<T>& other)
  {
    return (*this) = other.get();
  }

  template <typename T>
  template <typename U>
  intrusive_ptr<T>&
  intrusive_ptr<T>::operator = (U* other)
  {
    // This is the only place where the counter is used.
    if (pointee_ != other)
    {
      // Decrement current pointee count and delete the object if it reaches 0.
      if (pointee_ && pointee_->counter_dec())
        delete pointee_;

      // Take the pointer, increment counter.
      pointee_ = other;
      if (pointee_)
        pointee_->counter_inc();
    }
    return *this;
  }

  template <typename T> bool
  intrusive_ptr<T>::operator == (const T* p) const
  {
    return pointee_ == p;
  }

  template <typename T> bool
  intrusive_ptr<T>::operator == (const intrusive_ptr<T> &p) const
  {
    return pointee_ == p.get();
  }

  template <typename T> bool
  intrusive_ptr<T>::operator != (const intrusive_ptr<T> &p) const
  {
    return pointee_ != p.get();
  }
  template <typename T> bool
  intrusive_ptr<T>::operator != (const T* p) const
  {
    return pointee_ != p;
  }

  template <typename T>
  template <typename U>
  intrusive_ptr<U>
  intrusive_ptr<T>::cast() const
  {
    U* ptr = dynamic_cast<U*>(pointee_);
    if (!ptr)
      throw std::bad_cast ();
    return ptr;
  }

  template <typename T>
  template <typename U>
  intrusive_ptr<U>
  intrusive_ptr<T>::unsafe_cast() const
  {
    U* ptr = dynamic_cast<U*>(pointee_);
    return ptr;
  }

  template <typename T>
  template <typename U>
  inline
  intrusive_ptr<U>
  intrusive_ptr<T>::unchecked_cast() const
  {
    assert(dynamic_cast<U*>(pointee_));
    return static_cast<U*>(pointee_);
  }

  template <typename T>
  template <typename U>
  bool
  intrusive_ptr<T>::is_a () const
  {
    return dynamic_cast<U*> (pointee_);
  }

  template <typename T>
  T*
  intrusive_ptr<T>::get() const
  {
    return pointee_;
  }

  template <typename T>
  T*
  intrusive_ptr<T>::operator->() const
  {
    assert(pointee_);
    return pointee_;
  }

  template <typename T>
  T&
  intrusive_ptr<T>::operator*() const
  {
    return *pointee_;
  }

  template <typename T>
  void
  intrusive_ptr<T>::reset()
  {
    (*this) = 0;
  }

  template <typename T>
  intrusive_ptr<T>::operator bool() const
  {
    return pointee_;
  }

#ifndef LIBPORT_NO_BOOST
  template <typename T>
  template <typename Archive>
  void
  intrusive_ptr<T>::save(Archive& ar, const unsigned int /* version */) const
  {
    ar & pointee_;
  }

  template <typename T>
  template <typename Archive>
  void
  intrusive_ptr<T>::load(Archive& ar, const unsigned int /* version */)
  {
    if (pointee_)
      pointee_->counter_dec();
    ar & pointee_;
    if (pointee_)
      pointee_->counter_inc();
  }

  template <typename T>
  template <typename Archive>
  void
  intrusive_ptr<T>::serialize(Archive& ar, const unsigned int version)
  {
    boost::serialization::split_member(ar, *this, version);
  }
#endif // !LIBPORT_NO_BOOST

  /*--------------------------.
  | Free standing functions.  |
  `--------------------------*/

  template <typename T>
  T*
  get_pointer(const intrusive_ptr<T>& p)
  {
    return p.get();
  }

  template <typename T>
  intrusive_ptr<T>
  make_intrusive_ptr(T* t)
  {
    return intrusive_ptr<T> (t);
  }

  template <typename U, typename T>
  intrusive_ptr<U>
  unsafe_cast(const intrusive_ptr<T>& p)
  {
    return p.unsafe_cast<U>();
  }

}

#endif // !LIBPORT_BOOST_INTRUSIVE_PTR_HXX
