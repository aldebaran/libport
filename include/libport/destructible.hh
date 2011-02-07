/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_DESTRUCTIBLE_HH
# define LIBPORT_DESTRUCTIBLE_HH

# include <libport/intrusive-ptr.hh>
# include <libport/lockable.hh>
# include <libport/condition.hh>

namespace libport
{
  /** Class handling delayed destruction of objects.
   *
   * When the user wants to delete an instance of a class inheriting from
   * Destructible, she calls the method destroy(). Effective deletion of
   * the instance will not take place before all destruction locks,
   * acquired with getDestructionLock(), are gone.
   * Effective deletion is handled by the virtual method doDestroy(), which
   * if not overriden calls the delete operator.
   * Classes inheriting from Destructible should put their destructor private
   * to avoid accidental overriding of the destroy() mechanism.
   *
   * If your class allows direct access to its delete operator, you must in
   * your destructor:
   *  - Call wasDestroyed()
   *  - Ensure no new destruction lock can be aquired
   *  - Call waitForDestructionPermission, or call checkDestructionPermission
   *    until it returns true.
   * This tasks must be performed also by all inherited classes to be safe in
   * case virtual methods are used.
   */
  class Destructible
  {
    public:
    class Lock: public libport::ThreadSafeRefCounted
    {
      public:
      Lock(Destructible& parent);
      ~Lock();
      private:
      Destructible& parent_;
    };

    typedef libport::intrusive_ptr<Lock> DestructionLock;

    Destructible();

    /// Prevent destruction of this object as long as the returned lock exists.
    DestructionLock getDestructionLock();

    /// Ask if a destruction request was received.
    bool pendingDestruction();

    /// Hold destruction lock \b l until destroyed.
    void link(DestructionLock l);

    /// Request the destruction of this object.
    void destroy();

    /// Virtual destructor.
    virtual ~Destructible();

    protected:

    /** Call this to warn Destructible that the destructor was called.
      * Only required if your object's destructor can be directly called.
      */
    void wasDestroyed();

    /** Block until no more destruction locks are acquired.
      * For this to make sense, it is your responsibility to ensure
      * no new destruction lock can be acquired from this point on.
      */
    void waitForDestructionPermission();

    /// Returns true if no destruction locks are acquired.
    bool checkDestructionPermission();

    /// Override this function to change the effective destruction behavior.
    virtual void doDestroy();

    /// Release all held destruction locks
    void unlinkAll();
    private:
    inline void take();
    inline void release();
    Condition canDestroy_;
    libport::Lockable threadLock_;
    bool destructionPending_;
    bool destructionEnacted_;
    int count_;
    std::vector<DestructionLock> links_;
    friend class Lock;
  };
}

# include "libport/destructible.hxx"

#endif
