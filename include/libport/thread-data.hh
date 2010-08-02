/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_THREAD_DATA_HH
# define LIBPORT_THREAD_DATA_HH

# include <libport/local-data.hh>

// smallest header for thread_specific_ptr
# include <boost/thread/tss.hpp>

namespace libport
{
  /// \class shortcut to boost implementation.
  template <typename T>
  class ThreadSpecificPtr
    : public boost::thread_specific_ptr<T>
  {
  };

  /// \class shortcut to boost implementation that does not delete the backend.
  template <typename T>
  class UnmanagedThreadSpecificPtr
    : public boost::thread_specific_ptr<T>
  {
  public:
    UnmanagedThreadSpecificPtr()
    : boost::thread_specific_ptr<T>(&noop)
    {}
    static void noop(T*) {}
  };

  namespace localdata
  {
    /// \brief Define Thread encapsulation type.
    struct Thread {};

    /// \brief Define types used by local data for thread encapsulation.
    template <typename T>
    struct Wrapper<T, Thread>
    {
      typedef T target;
      typedef T type;
      typedef ThreadSpecificPtr<type> container;
    };
  }

}

#endif
