/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef SCHED_EXCEPTION_HXX
# define SCHED_EXCEPTION_HXX

# include <libport/cstdlib>
# include <libport/cassert>

namespace sched
{

  inline
  exception::~exception() throw()
  {
  }

  inline const char*
  exception::what() const throw()
  {
    return "unknown exception";
  }

  inline
  void exception::rethrow() const
  {
    rethrow_();
    // GCC cannot guarantee that an overriden virtual method
    // will not return, so help it here.
    pabort("rethrow_ returned");
  }

} // namespace sched

#endif // SCHED_EXCEPTION_HXX
