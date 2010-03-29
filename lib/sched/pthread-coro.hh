/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef SCHED_PTHREAD_CORO_HH
# define SCHED_PTHREAD_CORO_HH

# include <libport/config.h>

// Define LIBPORT_SCHED_CORO_OSTHREAD to use the os-thread implementation
// of coros.
# ifdef LIBPORT_SCHED_CORO_OSTHREAD
#  include <libport/semaphore.hh>

class Coro
{
public:
  Coro();
  libport::Semaphore sem_;
  bool started_;
  bool die_;
  pthread_t thread_;
};

#  include <sched/pthread-coro.hxx>

# endif // LIBPORT_SCHED_CORO_OSTHREAD

#endif // !SCHED_PTHREAD_CORO_HH
