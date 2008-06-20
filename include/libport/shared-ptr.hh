/**
 ** \file libport/shared-ptr.hh
 ** \brief Declaration of libport::shared_ptr.
 **/

#ifndef LIBPORT_SHARED_PTR_HH
# define LIBPORT_SHARED_PTR_HH

# include <libport/config.h>
# include <libport/meta.hh>
# include <libport/ref-counted.hh>

# ifndef LIBPORT_NO_BOOST
#  include <boost/serialization/serialization.hpp>
#  include <boost/shared_ptr.hpp>

namespace libport
{

  /** A shared_ptr wrapper.
    *
    * This implementation can be intrusive (storing the counter in the object)
    * if Intrusive is true. In this case T must provide the counter_inc and
    * counter_dec methods. counter_dec must return false if the counter reaches
    * 0. The default for the Intrusive parameter is to detect if T inherits from
    * libport::RefCounted.
    * In intrusive mode, it is safe to create multiple shared_ptr from the raw
    * pointer.
    * This implementation provides cast operators, and implicit constructors.
    * Its API is similar to boost::shared_ptr.
    */
  template <typename T, bool Intrusive = true>
  class shared_ptr : public boost::shared_ptr<T>
  {
    // This is the default (extrusive) implementation, wrapping
    // boost::shared_ptr.
  public:
    /// The parent class.
    typedef boost::shared_ptr<T> super_type;
    /// The type pointed to.
    typedef T element_type;

    using super_type::operator =;

    /// \name Constructors & Destructor.
    /// \{
    /** \brief Construct a new reference to the value pointed to by \a other.
     ** The new reference shares the property of the object with \a other. */
    template <typename U>
    shared_ptr (const shared_ptr<U, Intrusive>& other);

    /** \brief Copy constructor.
     **
     ** Although the implementation is subsumed by the previous, more
     ** generic one, the C++ standard still mandates this specific
     ** signature.  Otherwise, the compiler will provide a default
     ** implementation, which is of course wrong.  Note that the
     ** same applies for the assignment operator. */
    shared_ptr (const shared_ptr<T,Intrusive>& other);

    /*shared_ptr(const super_type& other)
    : super_type(other) {}*/
    /** \brief Construct a counted reference to a newly allocated object.
     ** The new reference takes the property of the object pointed to
     ** by \a p.  If \a p is NULL, then the reference is invalid and
     ** must be \c reset () before use. */
    shared_ptr (T* p = 0);

    /** \brief Destroy a reference.
     ** The object pointed to is destroyed iff it is not referenced anymore. */
    ~shared_ptr ();
    /// \}

    /// \name Equality operators.
    /// \{

    /** \brief Reference comparison.
     ** Returns true if this points to \a p. */
    bool operator== (const T* p) const;

    /** \brief Reference comparison.
     ** Returns false if this points to \a p. */
    bool operator!= (const T* p) const;

    /// \}

    /// \name Casts.
    /// \{

    /** \brief Cast the reference using a dynamic_cast.
     ** Return a new reference, possibly throwing an exception if the
     ** dynamic_cast is invalid.
     **/
    template <typename U> shared_ptr<U> cast () const;

    /** \brief Cast the reference using a dynamic_cast (unsafe).
     ** Return a new reference, possibly a NULL reference if the
     ** dynamic_cast is invalid.
     **/
    template <typename U> shared_ptr<U> unsafe_cast () const;
    /// \}

    /** \brief Test fellowship.
     ** Return true if the reference points to an object which is
     ** really of the specified type.
     **/
    template <typename U> bool is_a () const;
  };

  /// Intrusive shared_ptr implementation.
  template <typename T>
  class shared_ptr<T, true>
  {
  public:
    typedef T element_type;
    /// \name Constructors & Destructor.
    /// \{
    /** \brief Construct a new reference to the value pointed to by \a other.
     ** The new reference shares the property of the object with \a other. */
    template <typename U>
    shared_ptr (const shared_ptr<U, true>& other);

    /** \brief Copy constructor.
     **
     ** Although the implementation is subsumed by the previous, more
     ** generic one, the C++ standard still mandates this specific
     ** signature.  Otherwise, the compiler will provide a default
     ** implementation, which is of course wrong.  Note that the
     ** same applies for the assignment operator. */
    shared_ptr (const shared_ptr<T, true>& other);

    /** \brief Construct a counted reference to a newly allocated object.
     ** The new reference takes the property of the object pointed to
     ** by \a p.  If \a p is NULL, then the reference is invalid and
     ** must be \c reset () before use. */
    shared_ptr (T* p = 0);

    /** \brief Destroy a reference.
     ** The object pointed to is destroyed iff it is not referenced anymore. */
    ~shared_ptr ();
    /// \}

    /// \name Assignment operators.
    /// \{
      // This one is required or an incorrect implicit default will be used.
      shared_ptr& operator=(const shared_ptr<T, true>& other);
      template <typename U>
      shared_ptr& operator=(const shared_ptr<U, true>& other);

      template <typename U>
      shared_ptr& operator=(U* ptr);
    /// \}

    /// \name Equality operators.
    /// \{

    /** \brief Reference comparison.
     ** Returns true if this points to \a p. */
    bool operator== (const T* p) const;

    bool operator== (const shared_ptr<T,true>& p) const;
    bool operator!= (const shared_ptr<T,true>& p) const;

    /** \brief Reference comparison.
     ** Returns false if this points to \a p. */
    bool operator!= (const T* p) const;

    /// \brief bool cast operator to use the shared_ptr in boolean contexts.
    operator bool() const;
    /// \}

    /// \name Casts.
    /// \{

    /** \brief Cast the reference, using a dynamic_cast.
     ** Return a new reference, possibly throwing an exception if the
     ** dynamic_cast is invalid.
     **/
    template <typename U> shared_ptr<U> cast () const;

    /** \brief Cast the reference, using a dynamic_cast (unsafe).
     ** Return a new reference, possibly a NULL reference if the
     ** dynamic_cast is invalid.
     **/
    template <typename U> shared_ptr<U> unsafe_cast () const;
    /// \}

    /** \brief Test fellowship.
     ** Return true if the reference points to an object which is
     ** really of the specified type.
     **/
    template <typename U> bool is_a () const;


    /// \name boost::shared_ptr API.
    /// \{
      /// Get the underlying pointee.
      T* get() const;
      /// Equivalent to (*this) = 0;
      void reset();
      T* operator->() const;
      T& operator *() const;
    /// \}

  private:
    T* pointee_;

  private:
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version);
    static void counter_inc(shared_ptr<T, true>&);
  };


  /// For boost::mem_fn (and boost::bind) use.
  template<typename T, bool Intrusive>
  T*
  get_pointer(const shared_ptr<T, Intrusive>&);

  /// Simple wrapper to spare the explicit instantiation parameters.
  template <typename T>
  shared_ptr<T>
  make_shared_ptr(T* t);

  /// A more traditional syntax.
  ///
  /// Besides, it is easier to parse: GCC 4.0 (at least on OSX) cannot
  /// grok "children_->back().unsafe_cast<T>()" (it requires a tmp
  /// variable in between), but accepts
  /// "libport::unsafe_cast<T>(children_->back())".
  template <typename U, typename T>
  shared_ptr<U>
  unsafe_cast(const shared_ptr<T>& p);

}

#  include <libport/shared-ptr.hxx>

# endif // !LIBPORT_NO_BOOST
#endif // !LIBPORT_SHARED_PTR_HH
