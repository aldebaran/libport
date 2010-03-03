/*
 * Copyright (C) 2009, 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_REF_COUNTED_HH
# define LIBPORT_REF_COUNTED_HH

# include <boost/noncopyable.hpp>

namespace libport
{
  class RefCounted : boost::noncopyable
  {
    public:
      RefCounted ();
      virtual ~RefCounted();
      void counter_inc () const;
      bool counter_dec () const;

    protected:
      unsigned counter_get() const;

    private:
      mutable unsigned count_;
  };
}

# include <libport/ref-counted.hxx>

#endif
