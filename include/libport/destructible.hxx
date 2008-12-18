#ifndef LIBPORT_DESTRUCTIBLE_HXX
# define LIBORT_DESTRUCTIBLE_HXX

namespace libport
{
  inline Destructible::Lock::Lock(Destructible& parent)
    : parent_(parent)
  {
    parent_.take();
  }

  inline Destructible::Lock::~Lock()
  {
    parent_.release();
  }

  inline Destructible::Destructible()
    :destructionPending_(false)
     , destructionEnacted_(false)
     ,  count_(0)
  {
  }

  inline Destructible::DestructionLock
  Destructible::getDestructionLock()
  {
    return DestructionLock(new Lock(*this));
  }

  inline bool
  Destructible::pendingDestruction()
  {
    return destructionPending_;
  }

  inline void
  Destructible::destroy()
  {
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
  }

  inline void
  Destructible::doDestroy()
  {
    delete this;
  }

  inline void
  Destructible::take()
  {
    count_++;
  }

  inline void
  Destructible::release()
  {
    // Cant use blocklock: we must unlock before calling doDestroy.
    threadLock_.lock();
    count_--;
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
}

#endif
