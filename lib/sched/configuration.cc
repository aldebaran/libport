/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/config.h>
#include <sched/configuration.hh>

namespace sched
{

#if defined LIBPORT_COMPILATION_MODE_DEBUG
# if defined LIBPORT_COMPILATION_MODE_SPACE
#   define STACK_SIZE 256           // space, debug
# else
#   define STACK_SIZE 2048          // !space, debug
# endif
#else // ! DEBUG
# if defined LIBPORT_COMPILATION_MODE_SPACE
#   define STACK_SIZE 128           // space, !debug
# else
#   define STACK_SIZE 512           // !space, !debug
# endif
#endif

  Configuration configuration =
  {
    // Default is large enough until we enable a mechanism to dynamically
    // reallocate task space on demand.
    /* .default_stack_size = */   STACK_SIZE * 1024,
#if defined(__x86_64__)
    // 64 bits pointers eat more stack. SEGV have been observed in the
    // Urbi kernel (2.x/at/reg-trigger-from-condition.log).
    /* .minimum_stack_size = */   STACK_SIZE * 1024 / 6,
#else
    /* .minimum_stack_size = */   STACK_SIZE * 1024 / 8,
#endif
  };
}
