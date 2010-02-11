/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef SCHED_COROUTINE_HOOK_HH
# define SCHED_COROUTINE_HOOK_HH

# include <boost/function.hpp>
# include <sched/export.hh>
# include <sched/coroutine.hh>

typedef boost::function1<void, Coro*> coroutine_hook;

SCHED_API void add_coroutine_new_hook(coroutine_hook hook);
SCHED_API void add_coroutine_free_hook(coroutine_hook hook);

#endif
