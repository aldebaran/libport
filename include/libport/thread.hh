#ifndef LIBPORT_THREAD_HH
# define LIBPORT_THREAD_HH

# include <libport/config.h>
# include <boost/function.hpp>
# include <boost/bind.hpp>
# ifdef LIBPORT_WIN32
#  define WIN32
# endif

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

} // namespace libport

#endif // !LIBPORT_THREAD_HH
