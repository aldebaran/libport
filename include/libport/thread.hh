#ifndef LIBPORT_THREAD_HH
# define LIBPORT_THREAD_HH

# include <libport/config.h>
# include <libport/detect-win32.h>
# include <boost/function.hpp>
# include <boost/bind.hpp>
# include <boost/optional.hpp>

# if defined WIN32
#  include <libport/windows.hh>
# else
#  if !defined LIBPORT_HAVE_PTHREAD_H || !LIBPORT_HAVE_PTHREAD_H
#   error "pthread.h is required"
#  endif
# include <pthread.h>
# endif

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

  template<class T> void* startThread(T* obj)
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

  template<typename Res> class ThreadedCall
  {
    public:
    ThreadedCall():handle_(0) {};
    ThreadedCall(boost::function0<Res> f) {start(f);}
    void start(boost::function0<Res> f)
    {
      res_ = boost::none;
      handle_ = startThread(boost::bind(&ThreadedCall<Res>::wrap, this, f));
    }
    void wrap(boost::function0<Res> f)
    {
      res_ = f();
    }
    /// Return true if a job finished
    void clear() {res_ = boost::none; handle_ = 0;}
    bool finished() {return res_;}
    bool running() { return handle_ && !res_;}
    void wait() {if (!res_ && handle_) joinThread(handle_);}
    Res get() {return res_.get();}
    private:
    boost::optional<Res> res_;
    void* handle_;
  };

} // namespace libport

#endif // !LIBPORT_THREAD_HH
