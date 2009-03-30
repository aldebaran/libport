#ifndef LIBPORT_PTHREAD_HXX
# define LIBPORT_PTHREAD_HXX

# include <assert.h>
# include <boost/function.hpp>

typedef HANDLE (WINAPI *OT)(DWORD, BOOL, DWORD);

inline pthread_t
pthread_self() throw ()
{
  return GetCurrentThreadId();
}

inline int
pthread_create(pthread_t *thread, const pthread_attr_t *,
	       void *(*start_routine) (void *), void *arg)
{
  return !CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_routine,
		       arg, 0, thread);
}

inline int pthread_join(pthread_t thread, void** retval)
{
  HMODULE h = GetModuleHandle(TEXT("kernel32.dll"));
  assert(h);
  OT ot = (OT) GetProcAddress(h, "OpenThread");
  HANDLE hdl = ot(THREAD_ALL_ACCESS, 0, thread);
  int res = WAIT_FAILED == WaitForSingleObject(hdl, INFINITE)
    && GetExitCodeThread(hdl, (LPDWORD)*retval);
  return res;
}

#endif // !LIBPORT_PTHREAD_HXX
