/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_PTHREAD_H
# define LIBPORT_PTHREAD_H

# include <libport/config.h>
# include <libport/compiler.hh>
# include <libport/detect-win32.h>

# if defined WIN32
#  include <libport/windows.hh>
#  include <winbase.h>
# else
#  if !defined LIBPORT_HAVE_PTHREAD_H || !LIBPORT_HAVE_PTHREAD_H
#   error "pthread.h is required"
#  endif
# include <pthread.h>
# endif

# if defined WIN32
typedef DWORD pthread_t;
typedef void pthread_attr_t;

pthread_t pthread_self() throw ();

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
		   void *(*start_routine) (void *), void *arg);

int pthread_join(pthread_t thread, void** retval);

#  include <libport/pthread.hxx>
# endif

// On POSIX, pthread_* functions *return* the error code, but don't
// change errno.
#  define PTHREAD_RUN(Function, ...)            \
  do {                                          \
    if (int err = Function (__VA_ARGS__))       \
      {                                         \
        LIBPORT_USE(err);                       \
        errabort(err, #Function);               \
      }                                         \
  } while (false)

#endif // !LIBPORT_PTHREAD_HH
