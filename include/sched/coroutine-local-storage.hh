/*
 * Copyright (C) 2010-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef SCHED_COROUTINE_LOCAL_STORAGE_HH
# define SCHED_COROUTINE_LOCAL_STORAGE_HH

# include <boost/unordered_map.hpp>

# include <sched/coroutine.hh>

namespace sched
{
  template <typename T>
  class CoroutineLocalStorage
  {
  public:
    CoroutineLocalStorage();
    ~CoroutineLocalStorage();
    T& get();
    const T& get() const;
    T& operator * ();
    const T& operator * () const;
    T* operator -> ();
    const T* operator -> () const;
  private:
    typedef boost::unordered_map<Coro*, T*> map_type;
    map_type map_;
    void cleanup_(Coro* coro);
 };
}

# include <sched/coroutine-local-storage.hxx>

#endif
