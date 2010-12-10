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
 ** \file libport/intrusive-ptr.hxx
 ** \brief Implementation of libport::intrusive_ptr.
 **/

#ifndef LIBPORT_BOOST_INTRUSIVE_PTR_HXX
# define LIBPORT_BOOST_INTRUSIVE_PTR_HXX

# include <typeinfo>

# include <boost/functional/hash.hpp>

# include <libport/config.h>
# include <libport/cassert>

# include <libport/compiler.hh>

# if LIBPORT_ENABLE_SERIALIZATION
#  include <serialize/serialize.hh>
#  ifndef LIBPORT_NO_BOOST
#    include <boost/serialization/split_member.hpp>
#  endif
# endif

namespace libport
{
  template <typename T>
  template <typename U>
  ATTRIBUTE_ALWAYS_INLINE
  intrusive_ptr<T>::intrusive_ptr(const intrusive_ptr<U>& other)
    : pointee_(0)
  {
    *this = other;
  }

  template <typename T>
  ATTRIBUTE_ALWAYS_INLINE
  intrusive_ptr<T>::intrusive_ptr(const intrusive_ptr<T>& other)
    : pointee_(0)
  {
    *this = other;
  }

  template <typename T>
  ATTRIBUTE_ALWAYS_INLINE
  intrusive_ptr<T>::intrusive_ptr (T* p)
    : pointee_(0)
  {
    *this = p;
  }

  template <typename T>
  ATTRIBUTE_ALWAYS_INLINE
  intrusive_ptr<T>::~intrusive_ptr()
  {
    // This cast is required, or the compiler uses the intrusive_ptr ctor,
    // leading to an infinite loop.
    *this = (T*)0;
  }

  template <typename T>
  template <typename U>
  ATTRIBUTE_ALWAYS_INLINE
  intrusive_ptr<T>&
  intrusive_ptr<T>::operator = (const intrusive_ptr<U>& other)
  {
    return *this = other.get();
  }

  template <typename T>
  ATTRIBUTE_ALWAYS_INLINE
  intrusive_ptr<T>&
  intrusive_ptr<T>::operator = (const intrusive_ptr<T>& other)
  {
    return *this = other.get();
  }

  template <typename T>
  template <typename U>
  ATTRIBUTE_ALWAYS_INLINE
  intrusive_ptr<T>&
  intrusive_ptr<T>::operator = (U* other)
  {
    // This is the only place where the counter is used.
    if (pointee_ != other)
    {
      // Swap pointers before the destruction of its pointee to avoid
      // bad manipulation of this object when the pointee gets
      // destructed.
      T* previous = pointee_;

      // Take the pointer, increment counter.
      pointee_ = other;
      if (pointee_)
        pointee_->counter_inc();

      // Decrement previous pointee count and delete the object if it
      // reaches 0.
      if (previous && previous->counter_dec())
        delete previous;
    }
    return *this;
  }

  template <typename T>
  template <typename U>
  ATTRIBUTE_ALWAYS_INLINE
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
  ATTRIBUTE_ALWAYS_INLINE
  intrusive_ptr<U>
  intrusive_ptr<T>::unsafe_cast() const
  {
    U* ptr = dynamic_cast<U*>(pointee_);
    return ptr;
  }

  template <typename T>
  template <typename U>
  ATTRIBUTE_ALWAYS_INLINE
  intrusive_ptr<U>
  intrusive_ptr<T>::unchecked_cast() const
  {
    aver(dynamic_cast<U*>(pointee_));
    return static_cast<U*>(pointee_);
  }

  template <typename T>
  template <typename U>
  ATTRIBUTE_ALWAYS_INLINE
  bool
  intrusive_ptr<T>::is_a () const
  {
    return dynamic_cast<U*> (pointee_);
  }

  template <typename T>
  ATTRIBUTE_ALWAYS_INLINE
  T*
  intrusive_ptr<T>::get() const
  {
    return pointee_;
  }

  template <typename T>
  ATTRIBUTE_ALWAYS_INLINE
  intrusive_ptr<T>::operator T*() const
  {
    return pointee_;
  }

  template <typename T>
  ATTRIBUTE_ALWAYS_INLINE
  T*
  intrusive_ptr<T>::operator->() const
  {
    aver(pointee_);
    return pointee_;
  }

  template <typename T>
  ATTRIBUTE_ALWAYS_INLINE
  T&
  intrusive_ptr<T>::operator*() const
  {
    return *pointee_;
  }

  template <typename T>
  ATTRIBUTE_ALWAYS_INLINE
  void
  intrusive_ptr<T>::reset()
  {
    *this = 0;
  }

  template <typename T>
  std::ostream&
  operator << (std::ostream& out, const intrusive_ptr<T>& p)
  {
    return out << p.get();
  }

#ifndef LIBPORT_NO_BOOST
  template <typename T>
  template <typename Archive>
  ATTRIBUTE_ALWAYS_INLINE
  void
  intrusive_ptr<T>::save(Archive& ar, const unsigned int /* version */) const
  {
    ar & pointee_;
  }

  template <typename T>
  template <typename Archive>
  ATTRIBUTE_ALWAYS_INLINE
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
  ATTRIBUTE_ALWAYS_INLINE
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
  ATTRIBUTE_ALWAYS_INLINE
  T*
  get_pointer(const intrusive_ptr<T>& p)
  {
    return p.get();
  }

  template <typename T>
  ATTRIBUTE_ALWAYS_INLINE
  intrusive_ptr<T>
  make_intrusive_ptr(T* t)
  {
    return intrusive_ptr<T>(t);
  }

  template <typename U, typename T>
  ATTRIBUTE_ALWAYS_INLINE
  intrusive_ptr<U>
  unsafe_cast(const intrusive_ptr<T>& p)
  {
    return p.unsafe_cast<U>();
  }

  template <typename T>
  ATTRIBUTE_ALWAYS_INLINE
  std::size_t
  hash_value(const intrusive_ptr<T>& ptr)
  {
    return boost::hash_value(ptr.get());
  }

#ifdef LIBPORT_ENABLE_SERIALIZATION
  /*----------------.
  | Serialization.  |
  `----------------*/
  namespace serialize
  {
    template <typename T>
    struct serialize::BinaryOSerializer::Impl<libport::intrusive_ptr<const T> >
    {
      static void put(const std::string& name,
                      const libport::intrusive_ptr<const T>& ptr,
                      std::ostream& output, BinaryOSerializer& ser)
      {
        Impl<const T*>::put(name, ptr.get(), output, ser);
      }
    };

    template <typename T>
    struct serialize::BinaryOSerializer::Impl<libport::intrusive_ptr<T> >
    {
      static void put(const std::string& name,
                      const libport::intrusive_ptr<T>& ptr,
                      std::ostream& output, BinaryOSerializer& ser)
      {
        Impl<const T*>::put(name, ptr.get(), output, ser);
      }
    };

    template <typename T>
    struct serialize::BinaryISerializer::Impl<libport::intrusive_ptr<T> >
    {
      static intrusive_ptr<T> get(const std::string& name,
                                  std::istream& input, BinaryISerializer& ser)
      {
        return Impl<T*>::get(name, input, ser);
      }
    };
  }
#endif
}

#endif // !LIBPORT_BOOST_INTRUSIVE_PTR_HXX
