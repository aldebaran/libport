/**
 ** \file libport/shared-ptr.hxx
 ** \brief Implementation of libport::boost_shared_ptr.
 **/

#ifndef LIBPORT_BOOST_SHARED_PTR_HXX
# define LIBPORT_BOOST_SHARED_PTR_HXX

# include <libport/shared-ptr.hh>

namespace libport
{

  template <typename T>
  class dummy_ptr {
  public:
    static void counter_inc(shared_ptr<T, true>&) {}
  };

  /*-----------------.
  | Ctors and dtor.  |
  `-----------------*/

  template <typename T, bool I>
  template <typename U>
  shared_ptr<T,I>::shared_ptr (const shared_ptr<U,I>& other)
    : super_type (other)
  {
  }

  template <typename T, bool I>
  shared_ptr<T,I>::shared_ptr (const shared_ptr<T,I>& other)
    : super_type (other)
  {
  }

  template <typename T, bool I>
  shared_ptr<T,I>::shared_ptr (T* p)
    : super_type (p)
  {
  }

  template <typename T, bool I>
  shared_ptr<T,I>::~shared_ptr ()
  {
  }


  /*---------------------.
  | Equality operators.  |
  `---------------------*/

  template <typename T, bool I>
  bool
  shared_ptr<T,I>::operator== (const T* other) const
  {
    return this->get () == other;
  }

  template <typename T, bool I>
  bool
  shared_ptr<T,I>::operator!= (const T* other) const
  {
    return !(*this == other);
  }


  /*--------.
  | Casts.  |
  `--------*/

  template <typename T, bool I>
  template <typename U>
  shared_ptr<U>
  shared_ptr<T,I>::unsafe_cast () const
  {
    shared_ptr<U> res;
    (boost::dynamic_pointer_cast<U, element_type> (*this)).swap (res);
    return res;
  }

  template <typename T, bool I>
  template <typename U>
  shared_ptr<U>
  shared_ptr<T,I>::cast () const
  {
    if (!this->get () || !this->is_a<U> ())
      throw std::bad_cast ();
    return unsafe_cast<U> ();
  }

  template <typename T, bool I>
  template <typename U>
  bool
  shared_ptr<T,I>::is_a () const
  {
    return dynamic_cast<U*> (this->get ());
  }

  /*---------------------------.
  | Intrusive smart pointers.  |
  `---------------------------*/
  template <typename T>
  template <typename U>
  shared_ptr<T, true>::shared_ptr(const shared_ptr<U, true>& other)
    :pointee_(0)
  {
    (*this) = other;
  }
  template <typename T>
  shared_ptr<T, true>::shared_ptr(const shared_ptr<T, true>& other)
    :pointee_(0)
  {
    (*this) = other;
  }
  template <typename T>
  shared_ptr<T, true>::shared_ptr (T* p)
  :pointee_(0)
  {
    (*this) = p;
  }

  template <typename T>
  shared_ptr<T, true>::~shared_ptr()
  {
    // This cast is required, or the compiler uses the shared_ptr ctor,
    // leading to an infinite loop.
    (*this) = (T*)0;
  }

  template <typename T>
  template <typename U>
  shared_ptr<T, true>&
  shared_ptr<T, true>::operator = (const shared_ptr<U, true>& other)
  {
    return (*this) = other.get();
  }

  template <typename T>
  shared_ptr<T, true>&
  shared_ptr<T, true>::operator = (const shared_ptr<T, true>& other)
  {
    return (*this) = other.get();
  }

  template <typename T>
  template <typename U>
  shared_ptr<T, true>&
  shared_ptr<T, true>::operator = (U* other)
  {
    // This is the only place where the counter is used.

    // Decrement current pointee count and delete the object if it reaches 0.
    if (pointee_ && pointee_->counter_dec())
      delete pointee_;

    // Take the pointer, increment counter.
    pointee_ = other;
    if (pointee_)
      pointee_->counter_inc();
    return *this;
  }
  template <typename T> bool
  shared_ptr<T, true>::operator == (const T* p) const
  {
    return pointee_ == p;
  }

  template <typename T> bool
  shared_ptr<T, true>::operator == (const shared_ptr<T, true> &p) const
  {
    return pointee_ == p.get();
  }

  template <typename T> bool
  shared_ptr<T, true>::operator != (const shared_ptr<T, true> &p) const
  {
    return pointee_ != p.get();
  }
  template <typename T> bool
  shared_ptr<T, true>::operator != (const T* p) const
  {
    return pointee_ != p;
  }

  template <typename T>
  template <typename U>
  shared_ptr<U>
  shared_ptr<T, true>::cast() const
  {
    U* ptr = dynamic_cast<U*>(pointee_);
    if (!ptr)
      throw std::bad_cast ();
    return ptr;
  }

  template <typename T>
  template <typename U>
  shared_ptr<U>
  shared_ptr<T, true>::unsafe_cast() const
  {
    U* ptr = dynamic_cast<U*>(pointee_);
    return ptr;
  }

  template <typename T>
  template <typename U>
  bool
  shared_ptr<T, true>::is_a () const
  {
    return dynamic_cast<U*> (pointee_);
  }

  template <typename T>
  T*
  shared_ptr<T, true>::get() const
  {
    return pointee_;
  }

  template <typename T>
  T*
  shared_ptr<T, true>::operator->() const
  {
    return pointee_;
  }

  template <typename T>
  T&
  shared_ptr<T, true>::operator*() const
  {
    return *pointee_;
  }

  template <typename T>
  void
  shared_ptr<T, true>::reset()
  {
    (*this) = 0;
  }

  template <typename T>
  shared_ptr<T, true>::operator bool() const
  {
    return pointee_;
  }

  template <typename T>
  inline void
  shared_ptr<T, true>::counter_inc(shared_ptr<T, true>& self)
  {
    if (self.pointee_)
      self.pointee_->counter_inc();
  }

  template <typename T>
  template <typename Archive>
  void
  shared_ptr<T, true>::serialize(Archive& ar, const unsigned int /* version */)
  {
    ar & pointee_;
    typedef BOOST_DEDUCED_TYPENAME boost::mpl::if_<
      BOOST_DEDUCED_TYPENAME Archive::is_saving,
	dummy_ptr<T>,
	shared_ptr<T, true> >::type typex;
    typex::counter_inc(*this);
  }

  /*--------------------------.
  | Free standing functions.  |
  `--------------------------*/

  template <typename T, bool Intrusive>
  T*
  get_pointer(const shared_ptr<T, Intrusive>& p)
  {
    return p.get();
  }

  template <typename T>
  shared_ptr<T>
  make_shared_ptr(T* t)
  {
    return shared_ptr<T> (t);
  }


  template <typename U, typename T>
  shared_ptr<U>
  unsafe_cast(const shared_ptr<T>& p)
  {
    return p.unsafe_cast<U>();
  }

}

#endif // !LIBPORT_BOOST_SHARED_PTR_HXX
