#ifndef LIBPORT_REF_COUNTED_HH
# define LIBPORT_REF_COUNTED_HH

# include <boost/noncopyable.hpp>

namespace libport
{
  class RefCounted : boost::noncopyable
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
