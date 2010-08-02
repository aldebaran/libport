/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

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
    /// Check that inst is a registered instance.
    bool check(T* inst) const;
  private:
    static set_type instances_;
  };
}

# include <libport/instance-tracker.hxx>

#endif
