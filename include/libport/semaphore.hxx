/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_SEMAPHORE_HXX
# define LIBPORT_SEMAPHORE_HXX

# include <libport/semaphore.hh>

namespace libport
{

  inline void
  Semaphore::operator++(int)
  {
    operator++();
  }

  inline void
  Semaphore::operator--(int)
  {
    operator--();
  }

} // namespace libport

#endif // !LIBPORT_SEMAPHORE_HXX
