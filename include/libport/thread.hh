/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
#ifndef LIBPORT_THREAD_HH
# define LIBPORT_THREAD_HH

# include <boost/function.hpp>
# include <boost/optional.hpp>

# include <libport/pthread.h>

namespace libport
{

  pthread_t
  startThread(boost::function0<void> func);

  template<class T>
  pthread_t
  startThread(T* obj);

  template<class T>
  pthread_t
  startThread(T* obj, void (T::*func)(void));

  template<typename Res>
  class ThreadedCall
  {
  public:
    ThreadedCall();
    ThreadedCall(boost::function0<Res> f);
    void start(boost::function0<Res> f);
    void wrap(boost::function0<Res> f);

    void clear();
    /// Return true if a job finished.
    bool finished() const;
    bool running() const;
    void wait();
    Res get();

  private:
    boost::optional<Res> res_;
    pthread_t handle_;
  };

} // namespace libport

# include <libport/thread.hxx>

#endif // !LIBPORT_THREAD_HH
