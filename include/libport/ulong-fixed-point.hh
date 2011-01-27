/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
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

# include <boost/operators.hpp>

# include <libport/config.h>
# ifdef LIBPORT_ENABLE_SERIALIZATION
#  include <serialize/serialize.hh>
# endif

//static bool pdbg_ = getenv("DBG_FPOINT");

namespace libport
{
  /** Fixed point implementation on a long. Point is the number of
   * bits of the DECIMAL part, ie <sz(long)-point>.<point>
   * represnetation.  For this implementation to work, a long long
   * type of size twice the long type must be available.
   */
  template<int point>
  class ULongFixedPoint : boost::operators<ULongFixedPoint<point> >
  , boost::multipliable<ULongFixedPoint<point>, long long>
  , boost::multipliable<ULongFixedPoint<point>, long>
  , boost::multipliable<ULongFixedPoint<point>, int>
  , boost::multipliable<ULongFixedPoint<point>, double>
  , boost::dividable<ULongFixedPoint<point>, long>
  , boost::dividable<ULongFixedPoint<point>, int>
  , boost::dividable<ULongFixedPoint<point>, double>
  , boost::subtractable<ULongFixedPoint<point>, long>
  , boost::subtractable<ULongFixedPoint<point>, int>
  , boost::less_than_comparable2<ULongFixedPoint<point>, int>
  , boost::less_than_comparable2<ULongFixedPoint<point>, long>
  , boost::equality_comparable2<ULongFixedPoint<point>, int>
  , boost::equality_comparable2<ULongFixedPoint<point>, long>
  {
  public:
    double getDoubleValue() const { return (double)v / (double)(1<<point);}
# ifndef NO_DOUBLE
    ULongFixedPoint(double d)
    {
      v = (base_t)(d* (double)(1<<point));
      if ((v<<1) - (base_t)((d* (double)(1<<(point+1)))))
	v++;
//      if (pdbg_)
//        std::cerr <<"point(double) " << (v >>point) <<',' << (v&FLOATPART_MASK)
//      << std::endl;
    }
    inline ULongFixedPoint<point> operator =(double d) { v = (long)(d* (double)(1<<point));return *this;}
    operator double() const
    {
      return getDoubleValue();
    }
    operator float() const
    {
      return getDoubleValue();
    }
# endif

#define UFLOAT_CAST(t) ULongFixedPoint(t d) { v = (long)d << point;}
    UFLOAT_CASTS
#undef UFLOAT_CAST

    ULongFixedPoint():v(0) {}
    ULongFixedPoint(const ULongFixedPoint<point> &b)
    {
      v=b.v;
//      if (pdbg_)
//        std::cerr <<"point(point) " << (v >>point) <<',' << (v&FLOATPART_MASK)
//      << std::endl;
    }
    ULongFixedPoint (long val, int shift)
    {
      v= val<<(point-shift);
    }


    inline ULongFixedPoint<point> operator =(ULongFixedPoint<point> b)
    {
      v=b.v;
//      if (pdbg_)
//        std::cerr <<"point=point " << (v >>point) <<',' << (v&FLOATPART_MASK)
//      << std::endl;
      return *this;
    }
    /// Initialize with the value val * 2^(-shift)
    operator bool() const
    {
      return v;
    };
    bool operator<(const ULongFixedPoint& x) const { return v < x.v;}
    bool operator==(const ULongFixedPoint& x) const { return v == x.v;}
    ULongFixedPoint& operator+=(const ULongFixedPoint& x)
    {
      v += x.v;
      return *this;
    }
    ULongFixedPoint& operator-=(const ULongFixedPoint& x)
    {
      v -= x.v;
      return *this;
    }
    ULongFixedPoint& operator*=(const ULongFixedPoint& x)
    {
      v =  ( (long long)v * (long long)x.v) >> DIVIDE_SHIFT;
      return *this;
    }
    ULongFixedPoint& operator/=(const ULongFixedPoint& x)
    {
      long long l=(long long)v<<LONG_NBIT;
      l /= (long long)x.v;
      v = (base_t)(l >> DIVIDE_SHIFT);
      if (l&DIVIDE_LOSTBIT)
        v++;
     return *this;
    }
    ULongFixedPoint& operator%=(const ULongFixedPoint& x)
    {
      long m = ((*this) / x).getValue();
      (*this) -= x*m;
      return *this;
    }
    ULongFixedPoint& operator|=(const ULongFixedPoint& x)
    {
      v |= x.v;
      return *this;
    }
    ULongFixedPoint& operator&=(const ULongFixedPoint& x)
    {
      v &= x.v;
      return *this;
    }
    ULongFixedPoint& operator^=(const ULongFixedPoint& x)
    {
      v ^= x.v;
      return *this;
    }
    ULongFixedPoint& operator++()
    {
      v += DIVIDE_VALUE;
      return *this;
    }
    ULongFixedPoint& operator--()
    {
      v -= DIVIDE_VALUE;
      return *this;
    }

    ULongFixedPoint operator -() const
    {
      ULongFixedPoint res;
      res.setV(-v);
      return res;
    }
    ULongFixedPoint operator +() const
    {
      return *this;
    }
    #ifdef LIBPORT_ENABLE_SERIALIZATION
    // Serialize as a double, so that we can communicate between float
    // implementations.
    template<typename T>
    ULongFixedPoint(serialize::ISerializer<T>& is)
    {
      double d = is.template unserialize<double>("");
      v = (long)d * DIVIDE_VALUE;
    }
    template<typename T>
    void serialize(serialize::OSerializer<T>& os) const
    {
      double d = (double)v / DIVIDE_VALUE;
      os.template serialize<double>("", d);
    }
    #endif
    /// return as a long the underlying value multiplied by 2^shift
    inline long getValue(int shift=0) const
    {
      return v >> (point-shift);
    }

#define UFLOAT_CAST(t)                                                   \
      inline operator t() const                                          \
      {                                                                  \
        return getValue();                                               \
      }                                                                  \
      inline bool operator < (t v) const                                 \
      {                                                                  \
        return getValue() < v;                                           \
      }                                                                  \
      inline bool operator > (t v) const                                 \
      {                                                                  \
        return getValue() > v;                                           \
      }                                                                  \
      inline bool operator == (t v) const                                \
      {                                                                  \
        return getValue() == v;                                          \
      }


    UFLOAT_CASTS
    #undef UFLOAT_CAST

    inline long ceil() const
    {
      long res = getValue() + ((v & FLOATPART_MASK)?1:0);
//      if (pdbg_)
//        std::cerr <<"pointceil "  << (v >>point) <<',' << (v&FLOATPART_MASK)
//          << ' '  << res << std::endl;
      return res;
    }
    inline long trunc() const
    {
      return getValue();
    }
    inline long round() const
    {
      return static_cast<long>(::round(getDoubleValue()));
    }

    inline double modf(double* iptr) const
    {
      *iptr = getValue();
      return (double)v / DIVIDE_VALUE - *iptr; // FIXME uh?
    }
    inline ULongFixedPoint pow(ULongFixedPoint e)
    {
      return std::pow(getDoubleValue(), e.getDoubleValue());
    }
    inline void setV(long val) { v=val;}
    inline long getV() {return v;}
    typedef int base_t;
    static const int FLOATPART_MASK = (1<<point)-1;
    static const int LONG_NBIT = sizeof(base_t)*8;
    static const int DIVIDE_VALUE = (1<<point);
    static const int DIVIDE_SHIFT = (sizeof(base_t)*8-point);
    static const int DIVIDE_LOSTBIT = 1<<(sizeof(base_t)*8-point-1);
  private:
    base_t v;
  };

  template<int point>
  std::istream&
  operator >>(std::istream & s,ULongFixedPoint<point> &u)
  {
    // temporary workaround
    double d;
    s >> d;
    u = d;
//    if (pdbg_)
//        std::cerr <<"point parse " << (u.getV() >>point) <<',' << (u.getV()&u.FLOATPART_MASK)
//      << std::endl;
    return s;
    /*
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
    //while (s.get()==' ')
    //  ;
    //s.unget();
    return s;
    */
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


  template<int point>
  inline
  libport::ULongFixedPoint<point>
  pow(libport::ULongFixedPoint<point> v, libport::ULongFixedPoint<point> e)
  {
    return v.pow(e);
  }

#define BOUNCE(Op)                                      \
  template<int point>                                   \
  inline long Op(libport::ULongFixedPoint<point> v)     \
  {                                                     \
    return v.Op();                                      \
  }

  BOUNCE(ceil);
  BOUNCE(round);
  BOUNCE(trunk);

}

template<int point>
inline double modf(libport::ULongFixedPoint<point> v, double *iptr)
{
  return v.modf(iptr);
}

#endif // !LIBPORT_ULONG_FIXED_POINT_HH
