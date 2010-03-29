/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_REF_PT_HH
# define LIBPORT_REF_PT_HH

namespace libport
{
  /// RefPt is used to make references to pointers.
  template <class T>
  class RefPt
  {
  public:
    RefPt(T* p_)
    {
      p = p_;
      cpt = 1;
    }

    ~RefPt()
    {
      delete p;
    }

    T* ref()
    {
      return p;
    }

    int liberate()
    {
      cpt--;
      if (cpt<=0)
      {
	delete p;
	p=0;
      }
      return cpt;
    }

    const RefPt *copy() const
    {
      cpt++;
      return this;
    }

    RefPt *copy()
    {
      cpt++;
      return this;
    }

  private:
    mutable int cpt;
    T* p;
  };


} // namespace libport

#endif // !LIBPORT_REF_PT_HH
