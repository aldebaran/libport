/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_PTHREAD_HXX
# define LIBPORT_PTHREAD_HXX

# include <libport/cassert>
# include <boost/function.hpp>

typedef HANDLE (WINAPI *OT)(DWORD, BOOL, DWORD);

inline int
pthread_attr_init(pthread_attr_t *attr)
{
  *attr = 0; /* default value to unknown stack value */
  return 0;
}

inline int
pthread_attr_destroy(pthread_attr_t *)
{
  return 0;
}


inline int
pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
  if (stacksize < PTHREAD_STACK_MIN)
    return EINVAL;
  *attr = stacksize;
  return 0;
}

inline pthread_t
pthread_self() throw ()
{
  return GetCurrentThreadId();
}

inline int
pthread_create(pthread_t *thread, const pthread_attr_t *attr,
	       void *(*start_routine) (void *), void *arg)
{
  size_t stacksize = 0;
  if (attr)
    stacksize = *attr;
  return !CreateThread(NULL, stacksize,
                       (LPTHREAD_START_ROUTINE)start_routine,
		       arg, 0, thread);
}

inline int
pthread_join(pthread_t thread, void** retval)
{
  HMODULE h = GetModuleHandle(TEXT("kernel32.dll"));
  aver(h);
  OT ot = (OT) GetProcAddress(h, "OpenThread");
  HANDLE hdl = ot(THREAD_ALL_ACCESS, 0, thread);
  int res = (WAIT_FAILED == WaitForSingleObject(hdl, INFINITE)
             && GetExitCodeThread(hdl, (LPDWORD)*retval));
  return res;
}

#endif // !LIBPORT_PTHREAD_HXX
