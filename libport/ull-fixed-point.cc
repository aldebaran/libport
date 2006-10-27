#include <iostream>
#include <cmath>
#include "libport/ull-fixed-point.hh"

namespace urbi
{

  std::ostream&
  operator <<(std::ostream &s, ULLFixedPoint u)
  {
    long long uh  = (u.v >> LONG_NBIT);
    unsigned long long ul = u.v&((1LL<<LONG_NBIT)-1);
    bool neg=false;
    if (uh<0)
      {
	uh++;
	ul = (1LL<<LONG_NBIT)-ul;
	neg=true;
      }
    if (neg && uh==0)
      s<<"-";
    s<< uh <<".";
    u.rawSet(ul);
    const int limit = 1+(LONG_NBIT*3)/10;
    ULLFixedPoint ten(10L);
    for (int i=0;i<limit;i++)
      {
	u=u*ten;
	s << (char)('0'+ u.getValue());
	u = u - u.getValue();
      }
    return s;
  }


  std::istream&
  operator >>(std::istream &s, ULLFixedPoint &u)
  {
    long ir=0;
    char c;
    bool negative = false;
    while (s.get()==' ')
      ;
    s.unget();
    //get sign if exists
    c=s.get();
    if (c=='-')
      negative = true;
    else if ((c>='0' && c<='9')||(c=='.'))
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
	  { //XXX ignoring locales
	    s.unget();
	    return s;
	  }
      }
    //get decimal part, counting number of characters
    int nc=0;
    long dr=0;
    long factor=1;
    const int limit = (LONG_NBIT*3)/10; //avoid storing too many of them
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

    while (s.get()==' ')
      ;
    s.unget();
    ULLFixedPoint t(dr);
    t = t / ULLFixedPoint(factor);
    u = u+t;
    if (negative)
      u = ULLFixedPoint(0L)-u;
    return s;
  }

#endif

}
