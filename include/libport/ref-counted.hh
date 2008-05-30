#ifndef LIBPORT_REF_COUNTED_HH
# define LIBPORT_REF_COUNTED_HH

namespace libport
{
  class RefCounted
  {
    public:
      RefCounted ();
      void counter_inc () const;
      bool counter_dec () const;

    private:
      mutable unsigned count_;
  };
}

# include <libport/ref-counted.hxx>

#endif
