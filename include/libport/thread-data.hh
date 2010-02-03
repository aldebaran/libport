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
# include <libport/specific-ptr.hh>
# include <libport/pthread.h>

namespace libport
{
  template <typename T>
  class ThreadKey
  {
  public:
    ThreadKey();
    ~ThreadKey();

    typedef pthread_t type;
    static type current();
    void register_free_hook(boost::function1<void, type> free);
    void set_hook();
    void cleanup();
  private:
    pthread_key_t key;
    boost::function1<void, type> free_;
  };

  template <typename T>
  struct ThreadSpecificPtr
    : SpecificPtr<T, ThreadKey<T> >
  {
  };

  namespace localdata
  {
    struct Thread {};

    template <typename T>
    struct Wrapper<T, Thread>
    {
      typedef T target;
      typedef T type;
      typedef ThreadSpecificPtr<type> container;
    };
  }

}

# include <libport/thread-data.hxx>

#endif
