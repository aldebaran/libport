/**
 ** \file libport/timer.hxx
 ** \brief Inline methods for libport/timer.hh.
 */

#ifndef LIBPORT_TIMER_HXX
# define LIBPORT_TIMER_HXX

# include <libport/timer.hh>
# include <libport/contract.hh>

namespace libport
{

  inline
  void
  timer::push(int i)
  {
    precondition(intmap.find(i) != intmap.end());
    push(intmap[i]);
  }

  inline
  void
  timer::pop(const std::string& task_name)
  {
    precondition(tasksmap[task_name] == tasks.top());
    pop();
  }

  inline
  void
  timer::pop(const int i)
  {
    pop(intmap[i]);
  }


  inline
  void
  timer::dump_on_destruction(std::ostream& out)
  {
    dump_stream = &out;
  }

  inline
  void
  timer::start()
  {
    total.start();
  }

  inline
  void
  timer::stop()
  {
    total.stop();
  }

  inline
  timer::time::time() :
    user(0), sys(0), wall(0)
  { }

  inline
  timer::time&
  timer::time::operator +=(const time& rhs)
  {
    wall += rhs.wall;
    user += rhs.user;
    sys += rhs.sys;
    return *this;
  }

} // namespace libport

#endif // !LIBPORT_TIMER_HXX
