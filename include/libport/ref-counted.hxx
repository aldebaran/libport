/*
 * Copyright (C) 2009, 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
#ifndef LIBPORT_REF_COUNTED_HXX
# define LIBPORT_REF_COUNTED_HXX

namespace libport
{

  inline RefCounted::RefCounted ()
    : count_(0)
  {}

  inline void RefCounted::counter_inc () const
  {
    ++count_;
  }

  inline bool RefCounted::counter_dec () const
  {
    return !--count_;
  }

  inline unsigned RefCounted::counter_get () const
  {
    return count_;
  }
}

#endif
