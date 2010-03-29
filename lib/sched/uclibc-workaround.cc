/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/* This file contains workarounds for two problems:
 *   - pthread_getspecific implementations relying on the stack adress to get
 *     a thread identifier, and not coping well with custom stacks.
 *   - Exception handling code relying on pthread_getspecific, and thus
 *     requiring one slot per coroutine.
 *
 * Those workarounds are currently enabled on arm uclibc, and disabled on all
 * other platforms, but this activation conditions might require adjustment.
 * They require the overriding of a few functions in the pthread library, and
 * access to the pthread sources.
 *
 * FIXME: When we will support builds without pthread, a case should be added
 * to disable all this stuff if pthread is not enabled.
 */

#include <sched/export.hh>
#include <sched/coroutine.hh>

// First prototype the function to avoid "no previous definition" warning.
// Obviously, don't make it static either!
SCHED_API void coroutine_force_hook_linkage();

void
coroutine_force_hook_linkage()
{}

#if defined __arm__
#include <features.h>
#if defined __UCLIBC__ && ! defined LIBPORT_SCHED_CORO_OSTHREAD
#include <libport/config.h>
#ifndef LIBPORT_HAVE_PTHREAD_SOURCES
# error "Pthread sources are required for building libsched on arm-uclibc"
#endif
#include <iostream>
#include <vector>
#include <stdexcept>
#include <dlfcn.h>

extern "C" {
#include "linuxthreads/internals.h"
}

bool display = getenv("PS");

struct CoroInfo
{
  CoroInfo()
    : c(0)
    , specific(0)
  {}
  void init(Coro* cc)
  {
    c = cc;
    specific = new void*[PTHREAD_KEYS_MAX];
    memset(specific, 0, (PTHREAD_KEYS_MAX)*sizeof(void*));
  }
  void fini()
  {
    delete[] specific;
  }
  bool operator == (const CoroInfo& b) const
  {
    return c == b.c;
  }
  bool operator == (const Coro* cc) const
  {
    return c == cc;
  }
  mutable Coro* c;
  mutable void** specific;
};

static std::vector<CoroInfo> coroList;

static void new_hook(Coro* c)
{
  if (display)
    write(1,"new\n", 4);
  CoroInfo ci;
  ci.init(c);
  coroList.push_back(ci);
  if (display)
    write(1,"enew\n", 5);

}

static void free_hook(Coro* c)
{
  if (display)
    write(1, "free\n", 5);
  std::vector<CoroInfo>::iterator i
    = std::find(coroList.begin(), coroList.end(), c);
  std::swap(*i, coroList.back());
  coroList.back().fini();
  coroList.pop_back();
  if (display)
    write(1, "efree\n", 6);
}

extern "C" int __pthread_nonstandard_stacks;
extern "C" volatile int __pthread_handles_num;

static bool isInCoro()
{
   char * sp = CURRENT_STACK_FRAME;
  /* __pthread_handles[0] is the initial thread, __pthread_handles[1] is the
   *      manager threads handled specially in thread_self(), so start at
   *      2 */
  for (int i = 2; i < __pthread_handles_num /*PTHREAD_THREADS_MAX*/; i++)
  {
    pthread_handle h = __pthread_handles + i;
    if (sp <= (char *) h->h_descr && sp >= h->h_bottom)
      return false;
  }
 return (coroutine_current() != coroutine_main());
}

extern "C" SCHED_API void* pthread_getspecific(pthread_key_t k)
{
  Coro* cid = coroutine_current();
  bool isIn = isInCoro();
  if(isIn && !cid)
    exit(44);
  if (display)
  {
    char cmd[] = "pgs     \n";
    cmd[5] = '0' + (k%10);
    cmd[4] = '0' + (k/10);
    int id = (int)cid;
    if (id == -1)
      id = 8;
    cmd[7] = '0' + (isIn?id:9);
    write(1, cmd, 9);
  }
  typedef void*(*pgs_t)(pthread_key_t);
  static pgs_t pgs = 0;
  if (!pgs)
    pgs = (pgs_t) dlsym(RTLD_NEXT, "pthread_getspecific");

  if (!isIn)
    return pgs(k);
  CoroInfo& i = *std::find(coroList.begin(), coroList.end(), cid);
  //write(1, "resize\n", strlen("resize\n"));
  //write(1, "access\n", strlen("access\n"));
  void* v =  i.specific[k];
  if (display)
  {
    unsigned int iv = (unsigned int)v;
    char cmd[] = "d     \n";
    cmd[2] = '0' + ((iv/1000)%10);
    cmd[3] = '0' + ((iv/100)%10);
    cmd[4] = '0' + ((iv/10)%10);
    cmd[5] = '0' + ((iv/1)%10);
    write(1, cmd, 7);
  }
  return v;
}

extern "C" SCHED_API int pthread_setspecific(pthread_key_t k, const void* ptr)
{
  if (display)
  {
    char cmd[] = "pss     \n";
    cmd[5] = '0' + (k%10);
    cmd[4] = '0' + (k/10);
    write(1, cmd, 9);
    {
      unsigned int iv = (unsigned int)ptr;
      char cmd[] = "d     \n";
      cmd[2] = '0' + ((iv/1000)%10);
      cmd[3] = '0' + ((iv/100)%10);
      cmd[4] = '0' + ((iv/10)%10);
      cmd[5] = '0' + ((iv/1)%10);
      write(1, cmd, 7);
    }

  }
  typedef int(*pss_t)(pthread_key_t k, const void*);
  static pss_t pss = 0;
  if (!pss)
    pss = (pss_t)dlsym(RTLD_NEXT,"pthread_setspecific");

  if (!isInCoro())
    return pss(k, ptr);
  CoroInfo& i = *std::find(coroList.begin(), coroList.end(),
                           coroutine_current());
  i.specific[k] = (void*)ptr;
  return 0;
}

// Default implementation outputs junk if it cannot find a match.
extern "C"  SCHED_API pthread_descr __pthread_find_self (void)
{

  char * sp = CURRENT_STACK_FRAME;
  /* __pthread_handles[0] is the initial thread, __pthread_handles[1] is the
   *      manager threads handled specially in thread_self(), so start at
   *      2 */
  for (int i = 2; i < __pthread_handles_num /*PTHREAD_THREADS_MAX*/; i++)
  {
    pthread_handle h = __pthread_handles + i;
    if (sp <= (char *) h->h_descr && sp >= h->h_bottom)
      return h->h_descr;
  }

  return &__pthread_initial_thread;
}

namespace {
  class StaticInit
  {
  public:
    StaticInit()
    {
      // Set pthread_nonstandard_stacks internal flag to force pthred to call
      // __pthread_find_self
      __pthread_nonstandard_stacks = 1;
      // Hook coroutine creation and destruction api

      add_coroutine_new_hook(new_hook);
      add_coroutine_free_hook(free_hook);
    }
  };
  StaticInit static__init__;
}

#endif
#endif
