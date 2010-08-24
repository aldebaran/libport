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

# include <climits>

# include <libport/cassert>

namespace libport
{
  static const unsigned invalid_count = UINT_MAX;
  static const unsigned dying_count   = UINT_MAX - 1;

  inline RefCounted::RefCounted ()
    : count_(0)
  {}

  inline RefCounted::~RefCounted ()
  {
    count_ = invalid_count;
  }

  inline void RefCounted::counter_inc () const
  {
    aver(count_ != invalid_count);
    if (count_ == dying_count)
      return;
    ++count_;
  }

  // This method is used to avoid extra delete when you are using this
  // object inside the destructor.
  inline void RefCounted::counter_reset () const
  {
    aver(count_ == dying_count);
    count_ = 1;
  }

  inline bool RefCounted::counter_dec () const
  {
    aver(count_ != invalid_count);
    if (count_ == dying_count)
      return false;
    --count_;
    if (!count_)
    {
      count_ = dying_count;
      return true;
    }
    return false;
  }

  inline unsigned RefCounted::counter_get () const
  {
    aver(count_ != invalid_count);
    return count_;
  }
}

#endif
