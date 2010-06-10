/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
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

  /// Simple interface to run a function in a separate thread.
  template<typename Res>
  class ThreadedCall
  {
  public:
    ThreadedCall();

    /// Construct, and call start() on \a f.
    ThreadedCall(boost::function0<Res> f);

    /// Fire a new thread to compute \a f.
    void start(boost::function0<Res> f);

    /// Evaluate \a f (in the current thread), and store the result.
    void wrap(boost::function0<Res> f);

    /// Clear result and thread handle.
    void clear();

    /// Whether the job is finished.
    bool finished() const;

    /// Whether a thread is computing.
    bool running() const;

    /// Wait until the thread is done.
    void wait();

    /// Return the result of the last computed function.
    Res get();

  private:
    boost::optional<Res> res_;
    pthread_t handle_;
  };

} // namespace libport

# include <libport/thread.hxx>

#endif // !LIBPORT_THREAD_HH
