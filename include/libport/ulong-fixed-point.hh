/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_ULONG_FIXED_POINT_HH
# define LIBPORT_ULONG_FIXED_POINT_HH
# include <iostream>
# include <libport/cmath>
# include <algorithm>

namespace libport
{
  /** Fixed point implementation on a long. Point is the number of
   * bits of the DECIMAL part, ie <sz(long)-point>.<point>
   * represnetation.  For this implementation to work, a long long
   * type of size twice the long type must be available.
   */
  template<int point>
  class ULongFixedPoint
  {
  public:
#ifndef NO_DOUBLE
    ULongFixedPoint(double d)
    {
      v = (long)(d* (double)(1<<point));
      if (v<<1 - (long)((d* (double)(1<<(point+1)))))
	v++;
    }
    inline ULongFixedPoint<point> operator =(double d) { v = (long)(d* (double)(1<<point));return *this;}
    double getDoubleValue() { return (double)v / (double)(1<<point);}
#endif
    ULongFixedPoint(long d) {v=d<<point;}
    ULongFixedPoint():v(0) {}
    ULongFixedPoint(const ULongFixedPoint<point> &b) {v=b.v;}
    ULongFixedPoint (long val, int shift) {v= val<<(point-shift);}


    inline ULongFixedPoint<point> operator =(ULongFixedPoint<point> b) {v=b.v;}
    /// Initialize with the value val * 2^(-shift)
    inline ULongFixedPoint operator +(const ULongFixedPoint b) {ULongFixedPoint r;r.v=v+b.v;return r;}
    inline ULongFixedPoint operator -(const ULongFixedPoint b) {ULongFixedPoint r;r.v=v-b.v;return r;}
    inline ULongFixedPoint operator *(const ULongFixedPoint b) {ULongFixedPoint r; long long l=(long long)b.v*(long long)v; l >>= point; r.v=(long)l; return r;}
    inline ULongFixedPoint operator /(const ULongFixedPoint b) {ULongFixedPoint r; long long l=(long long)v<<LONG_NBIT; l/=(long long)b.v; r.v = (long)(l >> DIVIDE_SHIFT); if (l&DIVIDE_LOSTBIT) r.v++; return r;}

    /// return as a long the underlying value multiplied by 2^shift
    inline long getValue(int shift=0)
    {
      return v >> (point-shift);
    }


    inline void setV(long val) { v=val;}
    inline long getV() {return v;}
    static const int DIVIDE_SHIFT = (LONG_NBIT-point);
    static const int DIVIDE_LOSTBIT = 1<<(LONG_NBIT-point-1);
  private:
    long v;
  };

  template<int point>
  std::istream&
  operator >>(std::istream & s,ULongFixedPoint<point> &u)
  {
    long ir=0;
    char c;
    bool negative = false;
    while (s.get()==' ')
      ;
    s.unget();
    //no, eats one extra character s >> ir; // get integer part
    //get sign if exists
    c=s.get();
    if (c=='-')
      negative = true;
    else if (c>='0' && c<='9')
      s.unget();
    else if (c != '+')
      {
	s.setstate(std::ios_base::failbit);
	return s;
      }
    while (!s.eof())
      {
	c=s.get();
	if (!s)
	  break;
	if (c<'0' || c>'9')
	  {
	    s.unget();
	    break;
	  }
	ir=ir*10+(c-'0');
      }
    u = ir;

    if (!s)
      return s;
    if (!s.eof())
      {
	c=s.get();
	if (c!='.')
	  {
	    // XXX ignoring locales
	    s.unget();
	    return s;
	  }
      }
    //get decimal part, counting number of characters
    int nc=0;
    long dr=0;
    long factor=1;
    const int limit = 1+(point*3)/10; //avoid storing too many of them
    while (!s.eof())
      {
	c=s.get();
	if (!s)
	  break;
	if (c<'0' || c>'9')
	  {
	    s.unget();
	    break;
	  }
	if (nc != limit)
	  {
	    dr=dr*10+(c-'0');
	    nc++;
	    factor *=10;
	  }
      }

    ULongFixedPoint<point>  ip(ir);
    ULongFixedPoint<point>  t(dr);
    t = t / ULongFixedPoint<point>(factor);
    u = ip+t;
    if (negative) u=ULongFixedPoint<point>(0L)-u;
    while (s.get()==' ')
      ;
    s.unget();
    return s;
  }

  template<int point>
  std::ostream&
  operator <<(std::ostream &s, ULongFixedPoint<point> u)
  {
    s << u.getValue(); //output integral part
    u = u - u.getValue(); //get decimal part
    s << '.';
    const int limit = 1+(point*3)/10;
    for (int i=0;i<limit;i++)
      {
	u=u*10L;
	s << (char)('0'+ u.getValue());
	u = u - u.getValue();
      }
    return s;
  }

}

#endif // !LIBPORT_ULONG_FIXED_POINT_HH
