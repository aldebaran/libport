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
 ** \file libport/specific-ptr.hh
 ** \brief Declaration of libport::SpecificPtr.
 */

#ifndef LIBPORT_SPECIFIC_PTR_HH
# define LIBPORT_SPECIFIC_PTR_HH

# include <libport/hash.hh>

# include <boost/function.hpp>

namespace libport
{
  /** \brief This class is use to create a pointer specific to a process.
      This is a base to build thread-safeness and coroutine-safeness.

      Variability of processes are expressed through the \p Key parameter of
      this class.  The parameter \p T is the type of the expected object.
  */
  template <typename T, typename Key>
  class SpecificPtr
  {
  public:
    /// Type of the function used to clean up values when the process is
    /// calling its free hook.
    typedef boost::function1<void, T*> cleanFunction;

    /// Register the delete function as a cleaner for the object.
    SpecificPtr();
    /// Register an additionnal cleaner to avoid memory leaks.
    SpecificPtr(cleanFunction cleaner);

    /// Remove all mapped elements with the cleaner function.
    ~SpecificPtr();

    /// Get the mapped value, the returned value may be 0.  The value can be
    /// changed with the \fn reset fmethod.
    T* get();
    /// Similar to \fn get.
    T* operator->();
    /// Similar to \fn get except that the point is dereferenced.
    T& operator*();

    /// Replace the current value by a \a new_value for the instance defined
    /// by the static method \fn current of the \p Key parameter.  The
    /// previous value can be clean by definning the \a clean argument.
    /// This function return the previous value, and it returns 0 if it has
    /// been cleaned.
    T* reset(T* new_value = 0, bool clean = true,
             typename Key::type current = Key::current());

  private:
    // Do not use libport::map because its dump method expect to
    // serialized the key and the value.
    boost::unordered_map<typename Key::type, T *> map_;
    Key key_;
    cleanFunction cleaner_;

    /// Shortcut on top of \fn reset.
    void remove(typename Key::type current = Key::current());
  };
}

# include <libport/specific-ptr.hxx>

#endif
