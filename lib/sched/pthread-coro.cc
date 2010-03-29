/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <sched/pthread-coro.hh>

#if defined LIBPORT_SCHED_CORO_OSTHREAD

#  include <libport/cerrno>

#  include <libport/config.h>
#  include <libport/cassert>
#  include <libport/compiler.hh>
#  include <libport/semaphore.hh>
#  include <libport/thread.hh>

#  include <sched/configuration.hh>
#  include <sched/coroutine.hh>

/* Os-thread implementation of coroutines, using semaphores to ensure
   that only one coroutine is running at the same time.  */

/// FIXME: Needed to make bearclaw's code compile. Don't ask.
Coro* coro_current_;
Coro* coro_main_;

SCHED_API extern void (*coroutine_free_hook)(Coro*);
SCHED_API extern void (*coroutine_new_hook) (Coro*);

Coro::Coro()
  : started_(false)
  , die_(false)
{}

Coro*
coroutine_new(size_t)
{
  Coro *res = new Coro;
  if (coroutine_new_hook)
    coroutine_new_hook(res);
  return res;
}

void
coroutine_free(Coro* c)
{
  // If the thread has not been created, do nothing, otherwise free it.
  if (c->started_)
  {
    // First let c's thread terminate naturally
    c->die_ = true;
    c->sem_++;
    PTHREAD_RUN(pthread_join, c->thread_, 0);
    // Actually free it
    if (coroutine_free_hook)
      coroutine_free_hook(coro);
    delete c;
  }
}

void
coroutine_start(Coro* self, Coro* other,
                void (*callback)(void*), void* context)
{
  coro_current_ = other;
#if defined WIN32
  unsigned long id;
  void* r =
    CreateThread(NULL, 0,
                 reinterpret_cast<void* (*)(void*)>(callback), context,
                 0, &id);
#else
  // Adjust to the requested stack size.
  static pthread_attr_t attr;
  static bool first = true;
  if (first)
  {
    first = false;
    PTHREAD_RUN(pthread_attr_init, &attr);
    PTHREAD_RUN(pthread_attr_setstacksize,
                &attr, sched::configuration.default_stack_size);
  }

  PTHREAD_RUN(pthread_create, &other->thread_, &attr,
              reinterpret_cast<void* (*)(void*)>(callback), context);
#endif
  other->started_ = true;
  self->sem_--;
  coro_current_ = self;
}

#ifndef __APPLE__
static std::pair<void*, size_t>
get_stack()
{
  void* stack = 0;
  size_t size = 0;
  pthread_attr_t attr;
  PTHREAD_RUN(pthread_attr_init, &attr);
  PTHREAD_RUN(pthread_attr_getstack, &attr, &stack, &size);
  PTHREAD_RUN(pthread_attr_destroy, &attr);
  return std::make_pair(stack, size);
}
#endif

void*
coroutine_stack_addr(Coro*)
{
#ifdef __APPLE__
  // Undocumented, but works on OS X 10.5.
  return pthread_get_stackaddr_np(pthread_self());
#else
  return get_stack().first;
#endif
}

void
coroutine_switch_to(Coro* self, Coro* next)
{
  coro_current_ = next;
  next->sem_++;
  self->sem_--;
  coro_current_ = self;
  if (self->die_)
    pthread_exit(0);
}

void*
coroutine_current_stack_pointer(Coro*)
{
  uint8_t a;
  uint8_t *b = &a; // to avoid compiler warning about unused variables
  return b;
}

size_t
coroutine_stack_size(Coro*)
{
#ifdef __APPLE__
  // Undocumented, but works on OS X 10.5.
  return pthread_get_stacksize_np(pthread_self());
#else
  return get_stack().second;
#endif
}

void
coroutine_initialize_main(Coro* c)
{
  coro_main_ = c;
}

#endif //! LIBPORT_SCHED_CORO_OSTHREAD
