/*
 * Copyright (C) 2013 Aldebaran Robotics
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef SCHED_COROUTINE_BOOST_HXX
# define SCHED_COROUTINE_BOOST_HXX

#include <boost/context/all.hpp>


# include <libport/config.h>
# include <libport/debug.hh>
# include <libport/local-data.hh>
# include <libport/thread-data.hh>

# if ! defined __UCLIBC__ && defined LIBPORT_SCHED_MULTITHREAD
typedef ::libport::LocalSingleton<Coro*, ::libport::localdata::Thread>
  LocalCoroPtr;
# else
 /* Under uclibc, we need pthread local data to be coroutine-specific for proper
  * exception handling. So current_coro ptr cannot be a pthread local.
  */
typedef Coro* LocalCoroPtr;
# endif

// Hook stuff for uclibc-workaround
SCHED_API extern LocalCoroPtr coroutine_current_;
SCHED_API extern void (*coroutine_free_hook)(Coro*);
SCHED_API extern void (*coroutine_new_hook) (Coro*);
// Hack to force inclusion of hook object when the user links against a static
// libsched
SCHED_API void coroutine_force_hook_linkage();


typedef std::pair<void (*)(void*), void*> CoroStartContext;

static const bool coroutine_preserve_fpu = false;

inline void coroutine_starter(intptr_t vctx)
{
  CoroStartContext* octx = (CoroStartContext*)(void*)vctx;
  CoroStartContext ctx(*octx);
  delete octx;
  ctx.first(ctx.second);
}


// Implementation of coroutine wrapper API over boost::context
inline Coro* coroutine_new(size_t size)
{
  if (size == 0)
    size = sched::configuration.default_stack_size;
  void * sp1( malloc(size));
  Coro* res = boost::context::make_fcontext( (char*)sp1 + size, size, &coroutine_starter);
  if (coroutine_new_hook)
    coroutine_new_hook(res);
  return res;
}

inline void coroutine_free(Coro* coro)
{
  if (coroutine_free_hook)
    coroutine_free_hook(coro);
  free((char*)coro->fc_stack.sp - coro->fc_stack.size);
  // coro instead was on said stack, so nothing else to free
}


template<typename T>
void coroutine_start(Coro* self, Coro* other, void (*callback)(T*), T* context)
{
  CoroStartContext* ctx = new CoroStartContext;
  ctx->first = (void(*)(void*)) callback;
  ctx->second = context;
  coroutine_current_ = other;
  boost::context::jump_fcontext(self, other, (intptr_t)ctx, coroutine_preserve_fpu);
  coroutine_current_ = self;
}

inline void coroutine_switch_to(Coro* self, Coro* next)
{
  coroutine_current_ = next;
  boost::context::jump_fcontext(self, next, (intptr_t)0, coroutine_preserve_fpu);
  coroutine_current_ = self;
}

static void* coroutine_current_stack_pointer_helper()
#if __GNUC__ == 4
__attribute__ ((noinline))
#endif
;

void* coroutine_current_stack_pointer_helper()
{
  uint8_t a;
  uint8_t *b = &a; // to avoid compiler warning about unused variables
  return b;
}

inline void*
coroutine_current_stack_pointer(Coro*)
{
  return coroutine_current_stack_pointer_helper();
}

inline void* coroutine_stack_addr(Coro* self)
{
  return self->fc_stack.sp;
}

inline size_t coroutine_stack_size(Coro* self)
{
  return self->fc_stack.size;
}


inline void coroutine_initialize_main(Coro*) {}
inline Coro* coroutine_main() { return new Coro;}
inline Coro* coroutine_current() { return coroutine_current_;}



#endif
