#ifndef LIBPORT_REFCOUNTED_HH
# define LIBPORT_REFCOUNTED_HH

namespace libport
{
  class RefCounted
  {
    public:
    RefCounted():count_(0) {}
    void counter_inc() {++count_;}
    bool counter_dec() {return !--count_;}
    private:
    int count_;
  };
}
#endif
