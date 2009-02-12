#ifndef LIBPORT_CONDITION_HH
# define LIBPORT_CONDITION_HH
#include <libport/lockable.hh>
#include <libport/semaphore.hh>
#include <libport/utime.hh>
namespace libport
{
  /** Portable condition. Permits safe and low-cost wait until a condition
   * becomes true.
   *
   * You *must* own the lock *once* before calling wait or tryWait.
   *
   * Wait pattern:
   * {
   *   BlockLock bl(cond);
   *   if (!condition_check)
   *     cond.wait();
   *   act();
   * }
   * Update cond pattern:
   * {
   *    BlockLock bl(cond);
   *    modify_value;
   *    if (condition_check)
   *      cond.signal();
   * }
   */
  class Condition: public Lockable
  {
  public:
    Condition();
    /// Release lock, wait until condition is signaled, acquire lock.
    void wait();
    /// Same as above, but only wait for \b duration. Return true if signaled.
    bool tryWait(utime_t duration);
    /// Signal one thread wating on the condition.
    void signal();
    /// Signal all the threads waiting on the condition
    void broadcast();
  private:
# ifndef WIN32
    pthread_cond_t cond_;
# else
    /// True if last signaling was a broadcast
    bool broadcast_;
    /// Number of threads locked on the semaphore.
    size_t readers_count_;
    /// Semaphore on which threads wait on the condition.
    Semaphore  sem_;
# endif
  };

  /// Safely wait until expr becomes true.
  #define LIBPORT_COND_WAIT(cond, expr)   \
   do {                                   \
     BlockLock bl(cond);                  \
     if (!expr)                           \
       cond.wait();                       \
   } while(false)

  /// Safely execute updateCmd, notify waiters if checkExpr is true
  #define LIBPORT_COND_UPDATE(cond, updateCmd, checkExpr)  \
   do {                                                    \
     BlockLock bl(cond);                                   \
     bool was_true = checkExpr;                            \
     updateCmd;                                            \
     if (!was_true && checkExpr)                           \
       cond.signal();                                      \
   } while(false)
}

#include <libport/condition.hxx>

#endif
