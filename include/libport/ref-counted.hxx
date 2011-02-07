/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
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
  enum SpecialCountValues
  {
    invalid_count = INT_MIN,
    dying_count   = INT_MIN + 1,
  };

  inline
  RefCounted::RefCounted ()
    : count_(0)
  {}

  inline
  RefCounted::~RefCounted ()
  {
    // If we never were referenced (created on the stack, for
    // instance), count_ should be 0.
    // If we were, it should by dying_count: every reference taken
    // from the destructor must have been released.
    aver(count_ == dying_count || count_ == 0);
    count_ = invalid_count;
  }

  inline void
  RefCounted::counter_inc () const
  {
    aver(count_ != invalid_count);
    ++count_;
  }

  // This method is used to avoid extra delete when you are using this
  // object inside the destructor.
  inline void
  RefCounted::counter_reset () const
  {
    count_ = 1;
  }

  inline bool
  RefCounted::counter_dec () const
  {
    aver(count_ != invalid_count);
    --count_;
    if (!count_)
    {
      count_ = dying_count;
      return true;
    }
    return false;
  }

  inline RefCounted::count_type
  RefCounted::counter_get () const
  {
    aver(count_ != invalid_count);
    return count_;
  }

  inline
  RefCounted::Ward::Ward(RefCounted* ref_counted)
    : ref_counted_(*ref_counted)
  {
    ref_counted_.count_++;
  }

  inline
  RefCounted::Ward::~Ward()
  {
    ref_counted_.count_--;
  }

  inline void
  ThreadSafeRefCounted::counter_inc() const
  {
    libport::BlockLock bl(lock_);
    RefCounted::counter_inc();
  }
  inline bool
  ThreadSafeRefCounted::counter_dec() const
  {
    libport::BlockLock bl(lock_);
    return RefCounted::counter_dec();
  }
  inline RefCounted::count_type
  ThreadSafeRefCounted::counter_get() const
  {
    libport::BlockLock bl(lock_);
    return RefCounted::counter_get();
  }
  inline void
  ThreadSafeRefCounted::counter_reset() const
  {
    libport::BlockLock bl(lock_);
    RefCounted::counter_reset();
  }
}

#endif
