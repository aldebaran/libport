/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_UNIQUE_POINTER_HH
# define LIBPORT_UNIQUE_POINTER_HH

# include <set>
# include <libport/export.hh>

namespace libport
{
  /// Check whether a pointer was defined once only.
  class LIBPORT_API UniquePointer
  {
  public:
    UniquePointer();
    virtual ~UniquePointer();

    /// Register the pointer, pointer to the referenced entity.
    /// The null pointer is always considered unique.
    /// \return true iff it is the first time it is registered.
    template <typename T>
    bool operator() (T* t);

    template <typename T>
    bool operator() (T& t);

    void clear();

  private:
    typedef std::set<const void*> set_type;
    // The set of known pointers.
    set_type set_;
  };
}

# include <libport/unique-pointer.hxx>

#endif // !LIBPORT_UNIQUE_POINTER_HH
