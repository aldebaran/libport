/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_REF_COUNTED_HXX
# define LIBPORT_REF_COUNTED_HXX

# include <libport/cassert>

namespace libport
{
  inline RefCounted::RefCounted ()
    : count_(0)
  {}

  inline RefCounted::~RefCounted ()
  {}

  inline void RefCounted::counter_inc () const
  {
    aver(count_ != 0xFFFFFFFF);
    ++count_;
  }

  // This method is used to avoid extra delete when you are using this
  // object inside the destructor.
  inline void RefCounted::counter_reset () const
  {
    aver(count_ == 0xFFFFFFFF);
    count_ = 1;
  }

  inline bool RefCounted::counter_dec () const
  {
    aver(count_ != 0xFFFFFFFF);
    --count_;
    if (!count_)
    {
      count_ = 0xFFFFFFFF;
      return true;
    }
    return false;
  }

  inline unsigned RefCounted::counter_get () const
  {
    return count_;
  }
}

#endif
