#ifndef LIBPORT_THREAD_HH
# define LIBPORT_THREAD_HH

# include "libport/config.h"

# if defined WIN32 || defined LIBPORT_WIN32
#  define _WIN32_WINNT 0x0400
#  include <windows.h>
# else
#  if !defined LIBPORT_HAVE_PTHREAD_H || !LIBPORT_HAVE_PTHREAD_H
#   error "pthread.h is required"
#  endif
# include <pthread.h>
# endif

namespace urbi
{
  template<class T>
  class StartInfo
  {
  public:
    T* inst;
    void (T::*func)(void);
  };

# if defined WIN32 || defined LIBPORT_WIN32
  typedef DWORD ThreadStartRet;
#  define THREADSTARTCALL WINAPI
# else
  typedef void* ThreadStartRet;
#  define THREADSTARTCALL
# endif

  template<class T> ThreadStartRet THREADSTARTCALL
  _startThread2(void * data)
  {
    StartInfo<T> * st = (StartInfo<T>*)data;
    ((*st->inst).*st->func)();
    delete st;
    return static_cast<ThreadStartRet> (0);
  }

  template<class T> ThreadStartRet THREADSTARTCALL
  _startThread(void * data)
  {
    T * t = (T*)data;
    (*t)();
    return static_cast<ThreadStartRet> (0);
  }

  template<class T> void*
  startThread(T * obj, void (T::*func)(void))
  {
    StartInfo<T> * si = new StartInfo<T>();
    si->inst = obj;
    si->func = func;

# if defined WIN32 || defined LIBPORT_WIN32
    unsigned long id;
    void* r = CreateThread(NULL, 0, &_startThread2<T>, si, 0, &id);
# else
    pthread_t* pt = new pthread_t;
    pthread_create(pt, 0, &_startThread2<T>, si);
    void* r = pt;
# endif

    if (false)
      {
	//force instanciation
	_startThread2<T>(0);
      }

    return r;
  }

  template<class T> void* startThread(T* obj)
  {
# if defined WIN32 || defined LIBPORT_WIN32
    unsigned long id;
    void* r = CreateThread(NULL, 0, &_startThread<T>, obj, 0, &id);
# else
    pthread_t* pt = new pthread_t;
    pthread_create(pt, 0, &_startThread<T>, obj);
    void* r = pt;
# endif
    if (false)
      //force instanciation
      _startThread<T>(0);

    return r;
  }


  inline void joinThread(void* t)
  {
# if defined WIN32 || defined LIBPORT_WIN32
    WaitForSingleObject(t, INFINITE);
# else
    pthread_join(*(pthread_t*)t, 0);
# endif
  }

} // namespace urbi

#endif
