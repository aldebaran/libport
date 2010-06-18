/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** \file libport/timer.cc
 ** \brief Implementation for libport/timer.hh.
 */

#include <libport/cassert>
#include <libport/config.h>
#include <iostream>
#include <iomanip>

#ifdef LIBPORT_HAVE_SYS_TIMES_H
# include <sys/times.h>
#else

# include <libport/utime.hh>
# include <libport/detect-win32.h>
struct tms
{
  libport::utime_t tms_utime;  /* user time */
  libport::utime_t tms_stime;  /* system time */
  libport::utime_t tms_cutime; /* user time of dead children */
  libport::utime_t tms_cstime; /* system time of dead children */
};

# ifdef WIN32
# include <libport/windows.hh>
static libport::utime_t times(struct tms* t)
{
 //unit: 100 nanoseconds
 FILETIME ctime, etime, kerneltime, usertime;
 GetProcessTimes(GetCurrentProcess(), &ctime, &etime, &kerneltime, &usertime);
 t->tms_cutime = t->tms_cstime = 0;
 t->tms_utime = usertime.dwLowDateTime + static_cast<libport::utime_t>(usertime.dwHighDateTime) << 32;
 t->tms_stime = kerneltime.dwLowDateTime + static_cast<libport::utime_t>(kerneltime.dwHighDateTime) << 32;
 return libport::utime()*10LL;
}

/// FILETIME unit is 100 nanoseconds.
# define sysconf(i) 10000000LL
# endif

#endif

#include <libport/unistd.h>
#include <libport/timer.hh>

namespace libport
{

  /*-----------------.
  | timer::time_var.  |
  `-----------------*/

  timer::time_var::time_var ()
    : initial (true)
  { }

  void
  timer::time_var::start ()
  {
    struct tms t;

    begin.wall = times (&t);
    begin.user = t.tms_utime;
    begin.sys  = t.tms_stime;

    if (initial)
      {
	initial = false;
	first = begin;
      }
  }

  void
  timer::time_var::stop ()
  {
    struct tms t;

    last.wall = times (&t);
    last.user = t.tms_utime;
    last.sys = t.tms_stime;
    elapsed.wall += last.wall - begin.wall;
    elapsed.user += last.user - begin.user;
    elapsed.sys += last.sys - begin.sys;
  }

  bool
  timer::time_var::is_zero ()
  {
    return (true
	    && elapsed.wall == 0
	    && elapsed.user == 0
	    && elapsed.sys  == 0);
  }


  /*--------.
  | timer.  |
  `--------*/

  timer::timer()
  {}

  // Duplicate a timer. No tasks should be running.
  timer::timer(const timer& rhs)
    : intmap(rhs.intmap)
    , total(rhs.total)
  {
    aver(rhs.tasks.empty ());

    for (task_map_type::const_iterator i = rhs.tasksmap.begin ();
	 i != rhs.tasksmap.end (); ++i)
      if (tasksmap.find (i->first) == tasksmap.end ())
	tasksmap[i->first] = new time_var (*i->second);
  }


  timer::~timer ()
  {
    // Consider that if the tasks were not properly closed, then stop
    // was not invoked either.
    if (!tasks.empty ())
    {
      do
        pop ();
      while (!tasks.empty ());
      stop ();
    }

    foreach (const callback_type& c, destruction_callbacks_)
      c();

    // Deallocate all our time_var.
    for (task_map_type::iterator i = tasksmap.begin ();
	 i != tasksmap.end (); ++i)
      delete i->second;
  }

  void
  timer::destruction_hook(const callback_type& c)
  {
    destruction_callbacks_.push_back(c);
  }

  void
  timer::name (int i, std::string task_name)
  {
    intmap[i] = task_name;
  }

  std::ostream&
  timer::timeinfo (long time, long total_time, std::ostream& out) const
  {
    out << setiosflags (std::ios::left);
    out << std::setw (6) << std::setprecision (6)
	<< (float) (time) / clocks_per_sec;
    out << resetiosflags (std::ios::left);
    out << " (";
    out << std::setw (5) << std::setprecision (3)
	<< (total_time ?
	    (float) time * 100 / total_time :
	    (float) time);
    out << "%) ";
    return out;
  }


  std::ostream&
  timer::dump (std::ostream& out) const
  {
    out << "Execution times (seconds)" << std::endl;
    for (task_map_type::const_iterator i = tasksmap.begin ();
	 i != tasksmap.end (); ++i)
      {
	if (!(i->second->is_zero ()))
	  {
	    out << " " << i->first << std::setw (26 - i->first.length ());
	    out << ": ";
	    timeinfo (i->second->elapsed.user, total.elapsed.user, out);
	    out << "  ";
	    timeinfo (i->second->elapsed.sys, total.elapsed.sys, out);
	    out << "  ";
	    timeinfo (i->second->elapsed.wall, total.elapsed.wall, out);
	    out << std::endl;
	  }
      }
    out << std::endl;

    out << "Cumulated times (seconds)" << std::endl;
    for (task_map_type::const_iterator i = tasksmap.begin ();
	 i != tasksmap.end (); ++i)
      {
	if (0 != (i->second->last.wall - i->second->first.wall))
	  {
	    out << " " << i->first << std::setw (26 - i->first.length ());
	    out << ": ";
	    timeinfo (i->second->last.user - i->second->first.user,
		      total.elapsed.user, out);
	    out << "  ";
	    timeinfo (i->second->last.sys - i->second->first.sys,
		      total.elapsed.sys, out);
	    out << "  ";
	    timeinfo (i->second->last.wall - i->second->first.wall,
		      total.elapsed.wall, out);
	    out << std::endl;
	  }
      }
    out << std::endl;

    out << " TOTAL (seconds)"  << std::setw (11) << ": "

	<< setiosflags (std::ios::left) << std::setw (7)
	<< (float) total.elapsed.user / clocks_per_sec
	<< std::setw (11)
	<< "user,"

	<< std::setw (7)
	<< (float) total.elapsed.sys / clocks_per_sec
	<< std::setw (11)
	<< "system,"

	<< std::setw (7)
	<< (float) total.elapsed.wall / clocks_per_sec
	<< "wall"

	<< resetiosflags (std::ios::left) << std::endl;
    return out;
  }

  void
  timer::push (const std::string& task_name)
  {
    // if stack isn't empty, we set elapsed time for the current task
    if (!tasks.empty ())
      tasks.top ()->stop ();

    if (tasksmap.find (task_name) == tasksmap.end ())
      tasksmap[task_name] = new time_var;

    time_var* current = tasksmap[task_name];
    tasks.push (current);
    current->start ();
  }

  void
  timer::pop ()
  {
    aver(!tasks.empty ());

    // Set the Elapsed time for the task we are closing
    tasks.top ()->stop ();

    // Current task is removed of the stack
    tasks.pop ();

    // We set the start time of the previous task at current time
    if (!tasks.empty ())
      tasks.top ()->start ();
  }

  timer&
  timer::operator<< (const timer& rhs)
  {
    // No task should be running when merging timers.
    aver(rhs.tasks.empty ());

    for (task_map_type::const_iterator i = rhs.tasksmap.begin ();
	 i != rhs.tasksmap.end (); ++i)
      {
	if (tasksmap.find (i->first) == tasksmap.end ())
	  tasksmap[i->first] = new time_var (*i->second);
      }

    intmap.insert (rhs.intmap.begin (), rhs.intmap.end ());
    return *this;
  }

  const long timer::clocks_per_sec = sysconf(_SC_CLK_TCK);

} // namespace libport
