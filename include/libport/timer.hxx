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
 ** \file libport/timer.hxx
 ** \brief Inline methods for libport/timer.hh.
 */

#ifndef LIBPORT_TIMER_HXX
# define LIBPORT_TIMER_HXX

# include <libport/bind.hh>
# include <libport/cassert>
# include <boost/ref.hpp>

# include <libport/foreach.hh>
# include <libport/timer.hh>

namespace libport
{

  inline
  void
  timer::push(int i)
  {
    aver(intmap.find(i) != intmap.end());
    push(intmap[i]);
  }

  inline
  void
  timer::pop(const std::string&
# ifndef NDEBUG
             task_name
# endif
    )
  {
    assert_eq(tasksmap[task_name], tasks.top());
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
    destruction_hook(boost::bind(&timer::dump,
                                 this,
                                 boost::ref(out)));
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
  timer::time::time()
    : user(0), sys(0), wall(0)
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

  inline
  std::ostream&
  operator<<(std::ostream& o, const timer& t)
  {
    return t.dump(o);
  }

} // namespace libport

#endif // !LIBPORT_TIMER_HXX
