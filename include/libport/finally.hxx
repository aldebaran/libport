#ifndef LIBPORT_FINALLY_HXX
# define LIBPORT_FINALLY_HXX

namespace libport
{
  inline
  Finally::Finally()
    : actions_()
  {}

  inline
  Finally::Finally(const Action& a)
    : actions_()
  {
    *this << a;
  }

  inline
  Finally::~Finally()
  {
    foreach (const Action& a, actions_)
      a();
  }

  inline
  Finally& Finally::operator <<(const Action& a)
  {
    actions_.push_back(a);
    return *this;
  }
}


#endif
