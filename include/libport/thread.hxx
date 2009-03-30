#ifndef LIBPORT_THREAD_HXX
# define LIBPORT_THREAD_HXX

# include <libport/config.h>
# include <libport/detect-win32.h>
# include <boost/bind.hpp>

namespace libport
{
# if defined WIN32
  typedef DWORD ThreadStartRet;
#  define THREADSTARTCALL WINAPI
# else
  typedef void* ThreadStartRet;
#  define THREADSTARTCALL
# endif
  inline
  ThreadStartRet THREADSTARTCALL
  _startThread(void * data)
  {
    boost::function0<void> * s =(boost::function0<void>*) data;
    (*s)();
    delete s;
    return static_cast<ThreadStartRet> (0);
  }

  inline
  void* startThread(boost::function0<void> func)
  {
    boost::function0<void> * cp = new boost::function0<void>(func);
# if defined WIN32
    unsigned long id;
    void* r = CreateThread(NULL, 0, &_startThread, cp, 0, &id);
# else
    pthread_t* pt = new pthread_t;
    pthread_create(pt, 0, &_startThread, cp);
    void* r = pt;
# endif
    return r;
  }

  template<class T>
  void*
  startThread(T* obj)
  {
    return startThread(boost::bind(&T::operator(), obj));
  }

  template<class T> void*
  startThread(T * obj, void (T::*func)(void))
  {
    return startThread(boost::bind(func, obj));
  }

  inline void joinThread(void* t)
  {
# if defined WIN32
    WaitForSingleObject(t, INFINITE);
# else
    pthread_join(*(pthread_t*)t, 0);
# endif
  }

  inline pthread_t pthread_self()
  {
# if defined WIN32
    return GetCurrentThreadId();
# else
    return pthread_self();
# endif
  }

  inline bool checkMainThread(pthread_t tid)
  {
    return tid != pthread_self();
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
    res_ = boost::none;
    handle_ = startThread(boost::bind(&ThreadedCall<Res>::wrap, this, f));
  }

  template<typename Res>
  inline
  void
  ThreadedCall<Res>::wrap(boost::function0<Res> f)
  {
    res_ = f();
  }

  /// Return true if a job finished
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
    if (!res_ && handle_)
      joinThread(handle_);
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
