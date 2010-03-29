/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_WEAK_PTR_HH
# define LIBPORT_WEAK_PTR_HH

# include <libport/config.h>

# ifndef LIBPORT_NO_BOOST
#  include <boost/weak_ptr.hpp>


namespace libport
{

  /// A boost::weak_ptr wrapper.
  ///
  /// Compared to its super type, this implementation provides
  /// cast operators, and implicit constructors.
  template <typename T>
  class weak_ptr : public boost::weak_ptr<T>
  {
  public:
    /// The parent class.
    typedef boost::weak_ptr<T> super_type;
    /// The type pointed to.
    typedef T element_type;

    using super_type::operator =;

    /// \name Constructors & Destructor.
    /// \{
    /** \brief Construct a new reference to the value pointed to by \a other.
     ** The new reference shares the property of the object with \a other. */
    template <typename U>
    weak_ptr (const weak_ptr<U>& other);

    /** \brief Copy constructor.
     **
     ** Although the implementation is subsumed by the previous, more
     ** generic one, the C++ standard still mandates this specific
     ** signature.  Otherwise, the compiler will provide a default
     ** implementation, which is of course wrong.  Note that the
     ** same applies for the assignment operator. */
    weak_ptr (const weak_ptr<T>& other);

    /** \brief Construct a counted reference to a newly allocated object.
     ** The new reference takes the property of the object pointed to
     ** by \a p.  If \a p is NULL, then the reference is invalid and
     ** must be \c reset () before use. */
    weak_ptr ();

    /** \brief Destroy a reference.
     ** The object pointed to is destroyed iff it is not referenced anymore. */
    ~weak_ptr ();
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
    template <typename U> weak_ptr<U> cast () const;

    /** \brief Cast the reference (unsafe).
     ** Return a new reference, possibly a NULL reference if the
     ** dynamic_cast is invalid.
     **/
    template <typename U> weak_ptr<U> unsafe_cast () const;
    /// \}

    /** \brief Test fellowship.
     ** Return true if the reference points to an object which is
     ** really of the specified type.
     **/
    template <typename U> bool is_a () const;
  };

  /// Simple wrapper to spare the explicit instantiation parameters.
  template <typename T>
  weak_ptr<T>
  make_weak_ptr(T* t);

}

#  include <libport/weak-ptr.hxx>

# endif
#endif
