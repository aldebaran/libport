/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
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
# include <libport/lockable.hh>

namespace libport
{
  class RefCounted : boost::noncopyable
  {
    public:
      typedef signed int count_type;
      RefCounted ();
      virtual ~RefCounted();
      void counter_inc () const;
      bool counter_dec () const;
      count_type counter_get() const;

      // Prevent deletion in a scope, typically in a RefCounted
      // constructor.
      class Ward
      {
      public:
        Ward(RefCounted* ref_counted);
        ~Ward();

      private:
        RefCounted& ref_counted_;
      };

    protected:
      void counter_reset() const;

    private:
      mutable count_type count_;
  };

  class ThreadSafeRefCounted: RefCounted
  {
  public:
    void counter_inc () const;
    bool counter_dec () const;
    count_type counter_get() const;
  protected:
    void counter_reset() const;
    mutable libport::Lockable lock_;
  };
}

# include <libport/ref-counted.hxx>

#endif
