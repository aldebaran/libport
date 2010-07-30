/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_ULL_FIXED_POINT_HH
# define LIBPORT_ULL_FIXED_POINT_HH

# include <iosfwd>

namespace libport
{

  // FIXME: This is duplicated with uffloat.hh.
  static const int LONG_NBIT=sizeof(long)*8;
  static const int LONGLONG_NBIT=sizeof(long long)*8;

  static const int HALFLONG_NBIT=sizeof(long)*4;

  static const int LONG_NOSIGN_BIT_MASK=(1L<<(sizeof(long)*8-1))-1;
  static const int LONG_SIGN_BIT_MASK=(1L<<(sizeof(long)*8-1));
  static const long long LONG_VAL = (1LL<<LONG_NBIT);

  class ULLFixedPoint;
  typedef ULLFixedPoint ufloat;

  /// Fixed point implementation on a long long.
  /// Expected to be twice the size of a long
  class ULLFixedPoint
  {
  public:
    explicit ULLFixedPoint():v(0) {}
    ULLFixedPoint(const ULLFixedPoint &b):v(b.v) {}
    explicit ULLFixedPoint(double d) {v=(long long)(d*(double)LONG_VAL);}
    explicit ULLFixedPoint(long val) {v=(long long)val<<LONG_NBIT;}
    explicit ULLFixedPoint(int val) {v=(long long)val<<LONG_NBIT;}
    explicit ULLFixedPoint(unsigned int val) {v=(long long)val<<LONG_NBIT;}
    explicit ULLFixedPoint(unsigned long val) {v=(unsigned long long)val<<LONG_NBIT;}

    ULLFixedPoint operator =(double d) {v=(long long)(d*(double)LONG_VAL);return (*this);}
    ULLFixedPoint operator =(int val) {v=(long long)val<<LONG_NBIT;return (*this);}
    ULLFixedPoint operator =(unsigned int val) {v=(long long)val<<LONG_NBIT;return (*this);}
    ULLFixedPoint operator =(long val) {v=(long long)val<<LONG_NBIT;return (*this);}
    ULLFixedPoint operator =(long long val) {v=(long long)val<<LONG_NBIT;return (*this);}

    ULLFixedPoint operator =(unsigned long val) {v=(long long)val<<LONG_NBIT;return (*this);}


    void rawSet(long long val){v=val;}
    inline long getValue(int shift=0) const  {return v>>(LONG_NBIT-shift);}
    inline ULLFixedPoint operator -() const  {ULLFixedPoint r; r.v=-v; return r;}

    inline ULLFixedPoint operator >>=(int shift)
    {
      v = v>> shift;
      return *this;
    }
    inline ULLFixedPoint operator <<=(int shift)
    {
      v = v<< shift;
      return *this;
    }
    inline bool operator <(const ULLFixedPoint b) const {return v<b.v;}
    inline bool operator <=(const ULLFixedPoint b) const{return v<=b.v;}
    inline bool operator >(const ULLFixedPoint b) const {return v>b.v;}
    inline bool operator >=(const ULLFixedPoint b) const {return v>=b.v;}
    inline bool operator ==(const ULLFixedPoint b) const
    {
      long long l = v - b.v;
      if (l<0)
	l=-l;
      return l<4;
    }
    inline bool operator !=(const ULLFixedPoint b) const
    {
      long long l = v - b.v;
      if (l<0)
	l=-l;
      return l>4;
    }

    inline ULLFixedPoint operator >>(int b) const {ULLFixedPoint r;r.v=v>>b; return r;}
    inline ULLFixedPoint operator <<(int b) const {ULLFixedPoint r;r.v=v<<b; return r;}
    inline ULLFixedPoint operator +=(const ULLFixedPoint b) {v=v+b.v;return *this;}
    inline ULLFixedPoint operator -=(const ULLFixedPoint b) {v=v-b.v;return *this;}
    inline ULLFixedPoint operator *=(const ULLFixedPoint b) {return ((*this)=(*this)*b);}
    inline ULLFixedPoint operator /=(const ULLFixedPoint b) {return ((*this)=(*this)/b);}


    inline ULLFixedPoint operator +(const ULLFixedPoint b) const {ULLFixedPoint r;r.v=v+b.v;return r;}
    inline ULLFixedPoint operator -(const ULLFixedPoint b) const {ULLFixedPoint r;r.v=v-b.v;return r;}
    inline ULLFixedPoint operator *(const ULLFixedPoint b) const
    {
      ULLFixedPoint r;
      long long th = (v>>LONG_NBIT);
      unsigned long long  tl = v&((1LL<<LONG_NBIT)-1);
      //debug check
      /*
	long long d = tl + (((long long)th)<<LONG_NBIT);
	std::cerr <<th<<" + "<<tl<<" = "<<d<<"  = "<<v<<std::endl;
      */

      long long bh  = (b.v >> LONG_NBIT);
      unsigned long long bl = b.v&((1LL<<LONG_NBIT)-1);

      long long result =0;
      result = (long long)(th*bh)<<LONG_NBIT;
      result += (long long)(th*bl)+(long long)(tl*bh);
      result += (unsigned long long)(tl*bl)>>LONG_NBIT;
      r.v = result;
      return r;
    }

    inline ULLFixedPoint
    operator /(const ULLFixedPoint b) const
    {
      ULLFixedPoint result;
      unsigned long long sb;
      unsigned long long ut;
      if (b.v<0)
	sb = -b.v;
      else
	sb = b.v;
      if (v<0)
	ut = -v;
      else
	ut=v;
      int cbit = LONG_NBIT;
      while (sb > ut)
      {
	cbit--;
	sb = sb >>1;
      }
      while ((sb<<1) < ut)
      {
	cbit++;
	sb = sb << 1;
      }
      unsigned long long r=0;
      unsigned long long posmask = (1ULL<<cbit);
      while (posmask)
      {
	if (sb<=ut)
	{
	  r += posmask;
	  ut -= sb;
	}
	sb = sb >> 1;
	posmask >>= 1;
      }
      result.v=r;
      if (! ((v<0)==(b.v<0)))
	result.v = -result.v;
      return result;
    }

    inline ULLFixedPoint operator --() {return (*this)-=ULLFixedPoint(1); }
    inline ULLFixedPoint operator ++() {return (*this)+=ULLFixedPoint(1); }
    inline ULLFixedPoint operator --(int) {return (*this)-=ULLFixedPoint(1); }
    inline ULLFixedPoint operator ++(int) {return (*this)+=ULLFixedPoint(1); }

    /*
      inline ULLFixedPoint operator *(const int b) {return (*this)*ufloat(b);}
      inline ULLFixedPoint operator *(const unsigned int b) {return (*this)*ufloat(b);}
      inline ULLFixedPoint operator *(const long  b) {return (*this)*ufloat(b);}
      inline ULLFixedPoint operator *(const unsigned long b) {return (*this)*ufloat(b);}
      inline ULLFixedPoint operator *(const double  b) {return (*this)*ufloat(b);}
      inline ULLFixedPoint operator *(const float  b) {return (*this)*ufloat(b);}
    */
    double getDouble() const {return ((double)v)/double(1LL<<LONG_NBIT);}
    operator int() const {return getValue();};
    friend std::ostream& operator <<(std::ostream &s, ULLFixedPoint u);
    friend std::istream& operator >>(std::istream &s, ULLFixedPoint &u);
    friend ULLFixedPoint abs(ULLFixedPoint b);
    friend ULLFixedPoint trunc(ULLFixedPoint a);
  private:
    long long v;
  };

  inline ULLFixedPoint abs(ULLFixedPoint b)
  {
    ULLFixedPoint r(b);
    if (r.v<0)
      r.v = -r.v;
    return r;
  }

  /*
  inline ufloat atan(ufloat a) {return ufloat(atan(a.getDouble()));}
  inline ufloat pow(ufloat a, ufloat b) {return ufloat(pow(a.getDouble(), b.getDouble()));}
  inline ufloat exp(ufloat a) {return ufloat(exp(a.getDouble()));}
  inline ufloat log(ufloat a) {return ufloat(log(a.getDouble()));}
  inline ufloat sqrt(ufloat a) {return ufloat(sqrt(a.getDouble()));}
  inline ufloat fabs(ufloat a) {return (a>0)?a:-a;}
  */

  inline ufloat trunc(ufloat a)
  {
    ufloat b;
    b.v = a.v & (-1LL ^ ((1LL<<LONG_NBIT)-1LL));
    if (b.v<0)
      b++;
    return b;
  }

  inline ufloat fmod(ufloat a, ufloat b) {ufloat q = trunc(a/b); return a-q*b;}

}

#endif // !LIBPORT_ULL_FIXED_POINT_HH
