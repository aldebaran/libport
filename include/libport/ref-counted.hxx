#ifndef LIBPORT_REF_COUNTED_HXX
# define LIBPORT_REF_COUNTED_HXX

namespace libport
{

  inline RefCounted::RefCounted ()
    : count_(0)
  {}

  inline void RefCounted::counter_inc () const
  {
    ++count_;
  }

  inline bool RefCounted::counter_dec () const
  {
    return !--count_;
  }

  inline unsigned RefCounted::counter_get () const
  {
    return count_;
  }
}

#endif
