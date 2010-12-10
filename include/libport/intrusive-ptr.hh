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
 ** \file libport/intrusive-ptr.hh
 ** \brief Declaration of libport::intrusive_ptr.
 **/

#ifndef LIBPORT_INTRUSIVE_PTR_HH
# define LIBPORT_INTRUSIVE_PTR_HH

# include <libport/config.h>
# include <libport/meta.hh>
# include <libport/ref-counted.hh>

# ifndef LIBPORT_NO_BOOST
#  include <boost/serialization/serialization.hpp>
# endif

namespace libport
{

  /** A intrusive_ptr wrapper.
    *
    * This implementation is intrusive (storing the counter in the
    * object).  T must provide the counter_inc and counter_dec
    * methods. counter_dec must return false if the counter reaches 0.
    *
    * It is safe to create multiple intrusive_ptr from the raw
    * pointer. This implementation provides cast operators, and
    * implicit constructors.
    *
    * Its API is a superset of the boost::intrusive_ptr one.
    */
  template <typename T>
  class intrusive_ptr
  {
  public:
    typedef T element_type;
    /// \name Constructors & Destructor.
    /// \{
    /** \brief Construct a new reference to the value pointed to by \a other.
     ** The new reference shares the property of the object with \a other. */
    template <typename U>
    intrusive_ptr (const intrusive_ptr<U>& other);

    /** \brief Copy constructor.
     **
     ** Although the implementation is subsumed by the previous, more
     ** generic one, the C++ standard still mandates this specific
     ** signature.  Otherwise, the compiler will provide a default
     ** implementation, which is of course wrong.  Note that the
     ** same applies for the assignment operator. */
    intrusive_ptr (const intrusive_ptr<T>& other);

    /** \brief Construct a counted reference to a newly allocated object.
     ** The new reference takes the property of the object pointed to
     ** by \a p.  If \a p is NULL, then the reference is invalid and
     ** must be \c reset () before use. */
    intrusive_ptr (T* p = 0);

    /** \brief Destroy a reference.
     ** The object pointed to is destroyed iff it is not referenced anymore. */
    ~intrusive_ptr ();
    /// \}

    /// \name Assignment operators.
    /// \{
    // This one is required or an incorrect implicit default will be used.
    intrusive_ptr& operator=(const intrusive_ptr<T>& other);
    template <typename U>
    intrusive_ptr& operator=(const intrusive_ptr<U>& other);

    template <typename U>
    intrusive_ptr& operator=(U* ptr);
    /// \}

    /// \name Casts.
    /// \{

    /** \brief Cast the reference, using a dynamic_cast.
     ** Return a new reference, possibly throwing an exception if the
     ** dynamic_cast is invalid.
     **/
    template <typename U> intrusive_ptr<U> cast () const;

    /** \brief Cast the reference, using a dynamic_cast (unsafe).
     ** Return a new reference, possibly a NULL reference if the
     ** dynamic_cast is invalid.
     **/
    template <typename U> intrusive_ptr<U> unsafe_cast () const;
    /// \}

    /** \brief Cast the reference, using a static_cast.
     **/
    template <typename U> intrusive_ptr<U> unchecked_cast () const;
    /// \}

    /** \brief Test fellowship.
     ** Return true if the reference points to an object which is
     ** really of the specified type.
     **/
    template <typename U> bool is_a () const;


    /// \name boost::intrusive_ptr API.
    /// \{
    /// Get the underlying pointee.
    T* get() const;
    /// \brief plain pointer cast.
    operator T*() const;
    /// Equivalent to (*this) = 0;
    void reset();
    T* operator->() const;
    T& operator *() const;
    /// \}

  private:
    T* pointee_;

# ifndef LIBPORT_NO_BOOST
  private:
    /// Serialization.
    friend class boost::serialization::access;
    template <typename Archive>
    void load(Archive& ar, const unsigned int version);
    template <typename Archive>
    void save(Archive& ar, const unsigned int version) const;
    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version);
# endif
  };


  /// For boost::mem_fn (and boost::bind) use.
  template<typename T>
  T*
  get_pointer(const intrusive_ptr<T>&);

  /// Simple wrapper to spare the explicit instantiation parameters.
  template <typename T>
  intrusive_ptr<T>
  make_intrusive_ptr(T* t);

  /// A more traditional syntax.
  ///
  /// Besides, it is easier to parse: GCC 4.0 (at least on OSX) cannot
  /// grok "children_->back().unsafe_cast<T>()" (it requires a tmp
  /// variable in between), but accepts
  /// "libport::unsafe_cast<T>(children_->back())".
  template <typename U, typename T>
  intrusive_ptr<U>
  unsafe_cast(const intrusive_ptr<T>& p);

  template <typename T>
  std::ostream&
  operator << (std::ostream& out, const intrusive_ptr<T>& p);

  template <typename T>
  std::size_t
  hash_value(const intrusive_ptr<T>& ptr);
}

#  include <libport/intrusive-ptr.hxx>

#endif // !LIBPORT_INTRUSIVE_PTR_HH
