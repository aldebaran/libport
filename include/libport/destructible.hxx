/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_DESTRUCTIBLE_HXX
# define LIBORT_DESTRUCTIBLE_HXX

# include <libport/debug.hh>

namespace libport
{

  /*---------------------.
  | Destructible::Lock.  |
  `---------------------*/

  inline Destructible::Lock::Lock(Destructible& parent)
    : parent_(parent)
  {
    parent_.take();
  }

  inline Destructible::Lock::~Lock()
  {
    parent_.release();
  }


  /*---------------.
  | Destructible.  |
  `---------------*/

  inline Destructible::Destructible()
    : destructionPending_(false)
    , destructionEnacted_(false)
    , count_(0)
  {
  }

  inline Destructible::DestructionLock
  Destructible::getDestructionLock()
  {
    return DestructionLock(new Lock(*this));
  }

  inline
  bool
  Destructible::pendingDestruction() const
  {
    return destructionPending_;
  }

  inline void
  Destructible::destroy()
  {
    GD_CATEGORY(Libport.Destructible);
    GD_FINFO_TRACE("%p->Destructible::destroy", this);
    //Cant use BlockLock: must unlock before doDestroy.
    threadLock_.lock();
    destructionPending_ = true;
    if (!count_ && !destructionEnacted_)
    {
      destructionEnacted_ = true;
      threadLock_.unlock();
      doDestroy();
    }
    else
      threadLock_.unlock();
  }

  inline Destructible::~Destructible()
  {
    GD_CATEGORY(Libport.Destructible);
    GD_FINFO_TRACE("%p->Destructible::~Destructible", this);
  }

  inline void
  Destructible::doDestroy()
  {
    GD_CATEGORY(Libport.Destructible);
    GD_FINFO_TRACE("%p->Destructible::doDestroy", this);
    delete this;
  }

  inline void
  Destructible::take()
  {
    LIBPORT_COND_UPDATE(canDestroy_, count_++, false);
  }

  inline void
  Destructible::release()
  {
    // Cant use blocklock: we must unlock before calling doDestroy.
    threadLock_.lock();
    LIBPORT_COND_UPDATE(canDestroy_, count_--, !count_);
    if (!count_ && destructionPending_ && !destructionEnacted_)
    {
      destructionEnacted_ = true;
      threadLock_.unlock();
      doDestroy();
    }
    else
      threadLock_.unlock();
  }

  inline void
  Destructible::wasDestroyed()
  {
    destructionEnacted_ = true;
  }

  inline bool
  Destructible::checkDestructionPermission() const
  {
    return !count_;
  }

  inline void
  Destructible::waitForDestructionPermission()
  {
    LIBPORT_COND_WAIT(canDestroy_, !count_);
  }

  inline void
  Destructible::link(DestructionLock l)
  {
    links_.push_back(l);
  }

  inline void
  Destructible::unlinkAll()
  {
    links_.clear();
  }
}

#endif
