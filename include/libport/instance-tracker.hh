#ifndef LIBPORT_INSTANCE_TRACKER_HH
# define LIBPORT_INSTANCE_TRACKER_HH

# include <set>

namespace libport
{
  /* Track all instance of a given class
   *
   * By inheriting InstanceTracker<itself>, a class has a static
   * instances_get() that returns a set of all its current instances.
   */
  template <typename T>
  class InstanceTracker
  {
  public:
    typedef std::set<T*> set_type;

    InstanceTracker();
    ~InstanceTracker();
    static const set_type& instances_get();

  private:
    static set_type instances_;
  };
}

# include <libport/instance-tracker.hxx>

#endif
