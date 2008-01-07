/**
 ** \file libport/shared-ptr.hh
 ** \brief Declaration of libport::shared_ptr.
 **/

#ifndef LIBPORT_SHARED_PTR_HH
# define LIBPORT_SHARED_PTR_HH

# include "libport/config.h"

# ifndef LIBPORT_NO_BOOST
#  include <boost/shared_ptr.hpp>

namespace libport
{

  /// A boost::shared_ptr wrapper.
  ///
  /// Compared to its super type, this implementation provides
  /// cast operators, and implicit constructors.
  template <typename T>
  class shared_ptr : public boost::shared_ptr<T>
  {
  public:
    /// The parent class.
    typedef boost::shared_ptr<T> super_type;
    /// The type pointed to.
    typedef T element_type;

    /// \name Constructors & Destructor.
    /// \{
    /** \brief Construct a new reference to the value pointed to by \a other.
     ** The new reference shares the property of the object with \a other. */
    template <typename U>
    shared_ptr (const shared_ptr<U>& other);

    /** \brief Copy constructor.
     **
     ** Although the implementation is subsumed by the previous, more
     ** generic one, the C++ standard still mandates this specific
     ** signature.  Otherwise, the compiler will provide a default
     ** implementation, which is of course wrong.  Note that the
     ** same applies for the assignment operator. */
    shared_ptr (const shared_ptr<T>& other);

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

    /** \brief Cast the reference.
     ** Return a new reference, possibly throwing an exception if the
     ** dynamic_cast is invalid.
     **/
    template <typename U> shared_ptr<U> cast () const;

    /** \brief Cast the reference (unsafe).
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

  /// Simple wrapper to spare the explicit instantiation parameters.
  template <typename T>
  shared_ptr<T>
  make_shared_ptr(T* t);

}

#  include "libport/shared-ptr.hxx"

# endif // !LIBPORT_NO_BOOST
#endif // !LIBPORT_SHARED_PTR_HH
