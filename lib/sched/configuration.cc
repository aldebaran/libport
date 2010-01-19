/*
 * Copyright (C) 2009, 2010, Gostai S.A.S.
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
  Configuration configuration =
  {
    // Default is large enough until we enable a mechanism to dynamically
    // reallocate task space on demand.
    /* .default_stack_size = */   LIBPORT_URBI_KERNEL_STACK_SIZE * 1024,
    /* .minimum_stack_size = */   LIBPORT_URBI_KERNEL_STACK_SIZE * 1024 / 8,
  };
}
