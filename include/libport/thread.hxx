/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_THREAD_HXX
# define LIBPORT_THREAD_HXX

# include <libport/bind.hh>
# include <libport/cassert>
# include <libport/cerrno>
# include <libport/config.h>
# include <libport/detect-win32.h>

namespace libport
{
  inline
  void*
  _startThread(void *data)
  {
    boost::function0<void> *s = (boost::function0<void>*) data;
    (*s)();
    delete s;
    return 0;
  }

  inline
  pthread_t
  startThread(boost::function0<void> func)
  {
    boost::function0<void> *cp = new boost::function0<void>(func);
    pthread_t pt;
    PTHREAD_RUN(pthread_create, &pt, 0, &_startThread, cp);
    return pt;
  }

  template<class T>
  pthread_t
  startThread(T* obj)
  {
    return startThread(boost::bind(&T::operator(), obj));
  }

  template<class T>
  pthread_t
  startThread(T * obj, void (T::*func)(void))
  {
    return startThread(boost::bind(func, obj));
  }

  /*---------------.
  | ThreadedCall.  |
  `---------------*/

  template<typename Res>
  inline
  ThreadedCall<Res>::ThreadedCall()
    : handle_(0)
  {}

  template<typename Res>
  inline
  ThreadedCall<Res>::ThreadedCall(boost::function0<Res> f)
    : handle_(0)
  {
    start(f);
  }

  template<typename Res>
  inline
  void
  ThreadedCall<Res>::start(boost::function0<Res> f)
  {
    aver(!handle_);
    res_ = boost::none;
    handle_ = startThread(boost::bind(&ThreadedCall<Res>::wrap, this, f));
  }

  template<typename Res>
  inline
  void
  ThreadedCall<Res>::wrap(boost::function0<Res> f)
  {
    res_ = f();
#ifdef LIBPORT_HAVE_PTHREAD_SOURCES
    while (true)
      usleep(995000);
#endif
  }

  template<typename Res>
  inline
  void
  ThreadedCall<Res>::clear()
  {
    res_ = boost::none;
    handle_ = 0;
  }

  template<typename Res>
  inline
  bool
  ThreadedCall<Res>::finished() const
  {
    return res_;
  }

  template<typename Res>
  inline
  bool
  ThreadedCall<Res>::running() const
  {
    return handle_ && !res_;
  }

  template<typename Res>
  inline
  void
  ThreadedCall<Res>::wait()
  {
#ifdef LIBPORT_HAVE_PTHREAD_SOURCES
    while (!res_ && handle_)
      usleep(200000);
#else
    if (!res_ && handle_)
      PTHREAD_RUN(pthread_join, handle_, 0);
#endif
  }

  template<typename Res>
  inline
  Res
  ThreadedCall<Res>::get()
  {
    return res_.get();
  }

} // namespace libport

#endif // !LIBPORT_THREAD_HXX
