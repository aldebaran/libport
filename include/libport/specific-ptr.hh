/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_SPECIFIC_PTR_HH
# define LIBPORT_SPECIFIC_PTR_HH

# include <map>

# include <boost/function.hpp>

namespace libport
{
  template <typename T, typename Key>
  class SpecificPtr
  {
  public:
    typedef boost::function1<void, T*> cleanFunction;

    SpecificPtr();
    SpecificPtr(cleanFunction cleaner);

    ~SpecificPtr();

    T* get();
    T* operator->();
    T& operator*();

    T* reset(T* new_value = 0, bool clean = true,
             typename Key::type current = Key::current());

  private:
    // Do not use libport::map because its dump function expect to
    // serialized the key and the value.
    std::map<typename Key::type, T *> map_;
    Key key_;
    cleanFunction cleaner_;

    void remove(typename Key::type current = Key::current());
  };
}

# include <libport/specific-ptr.hxx>

#endif
