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
 ** \file libport/timer.hh
 ** \brief timer: Timing nested tasks.
 */

#ifndef LIBPORT_TIMER_HH
# define LIBPORT_TIMER_HH

# include <iostream>
# include <map>
# include <stack>
# include <string>
# include <vector>

# include <boost/function.hpp>

# include <libport/export.hh>

namespace libport
{

  /// Timing nested tasks.
  class LIBPORT_API timer
  {
  public:
    timer();
    timer(const timer& rhs);
    ~timer();

    /// Start a sub timer for a named task.
    /// \param name a constant string which is the task's name
    void push(const std::string& name);

    /// Start a sub timer with an integer which refers to a string.
    /// \see push()
    /// \see name()
    void push(int i);

    /// Stop the named task. Take a const string as parameter
    /// that make sure that what is popped is indeed what is on top.
    void pop(const std::string& task_name);

    /// \see pop()
    /// \see pop(const std::string& name)
    void pop(const int i);

    /// Stop the current task's timer (the last task pushed).
    void pop();

    /// Write results.
    /// \param out an ostream.
    std::ostream& dump(std::ostream& out = std::cerr) const;

    /// Write results when the timer is destroyed.
    /// \param out an ostream which defaults to cerr.
    void dump_on_destruction(std::ostream& out);

    /// the name function links an int and a constant string.
    void name(int, const std::string);

    /// the start function must be called at the beginning of the program
    /// to get the total time spend in the whole program.
    /// \see stop()
    void start();

    /// the stop function must be called at the end of the program.
    /// \see start()
    void stop();

    /// \brief Import timer.
    ///
    /// Import tasks defined in \a rhs. There must be not stacked task.
    /// Total execution time of \a rhs is ignored.
    timer& operator<<(const timer& rhs);

    /// Things to do when this timer is about to be destroyed.
    typedef boost::function0<void> callback_type;

    /// Register a new callback.
    void destruction_hook(const callback_type& c);

  private:
    class time_var;
    class time
    {
      friend class timer;
      friend class timer::time_var;
    public:
      time();
      time & operator +=(const time& rhs);

    private :
      long user;
      long sys;
      long wall;
    };

    class LIBPORT_API time_var
    {
    public:
      time_var();

      void start();
      void stop();

      bool is_zero();

      time begin;
      time elapsed;
      time first;
      time last;
      bool initial;
    };

    /// Format timing results.
    std::ostream& timeinfo(long, long, std::ostream&) const;

    typedef std::map<const std::string, time_var*> task_map_type;

    /// Store differents tasks you benched.
    task_map_type tasksmap;

    /// tasks is a stack in which tasks are benched in order.
    std::stack<time_var*> tasks;

    /// Map for int and string couples (to call push(int))
    std::map<int, std::string> intmap;

    /// Total time spend in the program
    /// \see start()
    /// \see stop()
    time_var total;

    /// Things to do when this timer is about to be destroyed.
    typedef std::vector<callback_type> callbacks_type;
    callbacks_type destruction_callbacks_;

    /// Number of clocks ticks per second, set according to the system
    /// timing function used.
    static const long clocks_per_sec;
  };

  std::ostream&
  operator<<(std::ostream& o, const timer& t);

} // namespace libport


# include <libport/timer.hxx>

#endif // !LIBPORT_TIMER_HH
