#ifndef LIBPORT_REFCOUNTED_HH
# define LIBPORT_REFCOUNTED_HH

namespace libport
{
  class RefCounted
  {
  public:
    RefCounted():count_(0) {}
    void counter_inc() const {++count_;}
    bool counter_dec() const {return !--count_;}
  private:
    mutable int count_;
  };
}
#endif
