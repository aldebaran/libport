#ifndef LIBPORT_DESTRUCTIBLE_HH
# define LIBPORT_DESTRUCTIBLE_HH

# include <libport/intrusive-ptr.hh>
# include <libport/lockable.hh>

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
   */
  class Destructible
  {
    public:
    class Lock: public libport::RefCounted
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

    /// Request the destruction of this object.
    void destroy();

    /// Virtual destructor.
    virtual ~Destructible();

    protected:

    /// Call this to warn Destructible that the destructor was called directly.
    void wasDestroyed();

    /// Override this function to change the effective destruction behavior.
    virtual void doDestroy();

    private:
    inline void take();
    inline void release();
    libport::Lockable threadLock_;
    bool destructionPending_;
    bool destructionEnacted_;
    int count_;
    friend class Lock;
  };
}

# include "libport/destructible.hxx"

#endif
