#include <sched/pthread-coro.hh>

#if defined LIBPORT_SCHED_CORO_OSTHREAD

#  include <cerrno>

#  include <libport/config.h>
#  include <libport/assert.hh>
#  include <libport/compiler.hh>
#  include <libport/semaphore.hh>
#  include <libport/thread.hh>

#  include <sched/configuration.hh>
#  include <sched/coroutine.hh>

/* Os-thread implementation of coroutines, using semaphores to ensure
   that only one coroutine is running at the same time.  */

Coro::Coro()
  : started_(false)
  , die_(false)
{}

Coro*
coroutine_new(size_t)
{
  return new Coro;
}

void
coroutine_free(Coro* c)
{
  // If the thread has not been created, do nothing, otherwise free it.
  if (c->started_)
  {
    c->die_ = true;
    c->sem_++;
    if (pthread_join(c->thread_, 0))
      errabort("pthread_join");
  }
}

void
coroutine_start(Coro* self, Coro* other,
                void (*callback)(void*), void* context)
{
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
    if (pthread_attr_init(&attr))
      errabort("pthread_attr_init");

    if (pthread_attr_setstacksize(&attr,
                                  sched::configuration.default_stack_size))
      errabort("pthread_attr_setstacksize");
  }

  if (pthread_create(&other->thread_, &attr,
		     reinterpret_cast<void* (*)(void*)>(callback), context))
    errabort("pthread_create");
#endif
  other->started_ = true;
  self->sem_--;
}

void*
coroutine_stack_addr(Coro*)
{
  void* res = 0;
  pthread_attr_t attr;
  if (pthread_attr_init(&attr))
    errabort("pthread_attr_init");
  if (pthread_attr_getstackaddr(&attr, &res))
    errabort("pthread_attr_getstackaddr");
  return res;
}

void
coroutine_switch_to(Coro* self, Coro* next)
{
  next->sem_++;
  self->sem_--;
  if (self->die_)
  {
    delete self;
    pthread_exit(0);
  }
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
  size_t res = 0;
  pthread_attr_t attr;
  if (pthread_attr_init(&attr))
    errabort("pthread_attr_init");
  if (pthread_attr_getstacksize(&attr, &res))
    errabort("pthread_attr_getstacksizeaddr");
  return res;
}

void
coroutine_initialize_main(Coro*)
{
}

#endif //! LIBPORT_SCHED_CORO_OSTHREAD
