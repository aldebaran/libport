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
