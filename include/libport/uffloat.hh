/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_UFFLOAT_HH
# define LIBPORT_UFFLOAT_HH
# include <iostream>
# include <libport/cmath>
# include <algorithm>

namespace libport
{
  enum { LONG_NBIT =     sizeof(long)*8 };
  enum { LONGLONG_NBIT = sizeof(long long)*8 };

  enum { HALFLONG_NBIT = sizeof(long)*4 };

  enum { LONG_NOSIGN_BIT_MASK = (1L << (sizeof(long)*8-1))-1 };
  enum { LONG_SIGN_BIT_MASK   = (1L << (sizeof(long)*8-1)) };

  static const long long LONG_VAL = 1LL << LONG_NBIT;

  class UFFloat;
  typedef UFFloat ufloat;

  /// Floating point implementation on a double long.
  ///
  /// Representation: v* 2^exp, with v normalized: high bit is sign,
  /// next bit has value 1-sign
  class UFFloat
  {
  public:
    UFFloat():v(0), e(-500) {};
    UFFloat(int val) {v=val;e=0;normalize();}
    UFFloat(long val) {v=val;e=0;normalize();}
    UFFloat(long val, int exp) {v=val;e=exp; normalize();}
    UFFloat(double d)
    {
      float f = (float)d;
      unsigned int l  = *((long*) ((void* )&f));
      long sign = l & 0x80000000;
      long mant = l&0x7fffff00;
      int exp = l&0xff - 127;
      //restore full mantisse
      mant = (mant>>1) + 0x40000000;
      //add sign
      if (sign)
	mant = 0-mant;
      v = mant;
      e = exp - 31;

    }
    UFFloat(float f)
    {
      unsigned int l  = *((long*) ((void* )&f));
      long sign = l & 0x80000000;
      long mant = l&0x7fffff00;
      int exp = l&0xff - 127;
      //restore full mantisse
      mant = (mant>>1) + 0x40000000;
      //add sign
      if (sign)
	mant = 0-mant;
      v = mant;
      e = exp - 31;
    }

    inline void normalize()
    {
      bool neg = (v<0);
      int pos = LONG_NBIT - 2;
      while (pos && (neg == (bool)(v & (1L<<pos))))
	pos--;
      v = v << (LONG_NBIT-pos-2); //bring sign bit to pos
      e = e- (LONG_NBIT-pos-2);
    }

    int getValue()
    {
      if (e>0)
	return v<<e;
      else
	return v>>(-e);
    }

    int getValue(int shift)
    {
      if (shift+e>0)
	return v<<(shift+e);
      else
	return v>>(-shift-e);
    }

    bool operator < (const UFFloat &b) const
    {
      //we can do better than actualy performing the substraction
      if (v<0 && b.v>0)
	return true;
      if (v>=0 && b.v<=0)
	return false;
      //same sign
      if (e != b.e)
	return ((e<b.e) != (v<0));
      return v<b.v;
    }

    bool operator > (const UFFloat &b) const
    {
      //we can do better than actualy performing the substraction
      if (v<=0 && b.v>=0)
	return false;
      if (v>0 && b.v<=0)
	return false;
      //same sign
      if (e != b.e)
	return ((e<b.e) != (v>=0));
      return v>b.v;
    }

    UFFloat operator *(const UFFloat &b) const
    {
      long long r=(long long)v*(long long)b.v;
      //locate first bit of dfferent value than first bit
      bool neg = (r<0);
      int pos = LONGLONG_NBIT - 2; //first non-sign bit
      while (pos && (neg == (bool)(r & (1LL<<pos))))
	pos--;
      UFFloat result;
      result.v = r >>std::max(0,pos-LONG_NBIT+2); //bring sign bit to pos
      result.e = e+b.e+std::max(0,pos-LONG_NBIT+2);
      if (pos < LONG_NBIT)
	result.normalize();
      if (result.v==0)
	result.e=-1000;
      return result;
    }

    UFFloat operator /(const UFFloat &b) const
    {
      long long num =((long long)v) << LONG_NBIT;
      long long res = num /(long long)b.v;
      bool neg = (res<0);
      int pos = LONGLONG_NBIT - 2;
      while (pos && (neg == (bool)(res & (1LL<<pos))))
	pos--;
      UFFloat result;
      int shift = pos - LONG_NBIT+2;
      if (shift>0)
	{
	  result.v = (long)(res >>shift); //bring sign bit to pos
	  result.e = e-b.e+shift-LONG_NBIT;
	}
      else
	{
	  result.v = res;
	  result.e = e-b.e+LONG_NBIT;
	  result.normalize();
	}
      if (result.v==0)
	result.e=-1000;
      return result;
    }


    UFFloat operator +(const UFFloat &b) const
    {
      UFFloat big,small;
      if (e<b.e)
	{
	  small = *this;
	  big = b;
	}
      else if (e>b.e)
	{
	  small = b;
	  big = *this;
	}
      else if (abs(v)>abs(b.v))
	{
	  // we need exact absolute value ordering
	  small = b;
	  big = *this;
	}
      else
	{
	  small = *this;
	  big = b;
	}
      long drift = big.e-small.e;
      if (drift>= LONG_NBIT)
	return big;
      unsigned long biguv = big.v & LONG_NOSIGN_BIT_MASK;
      unsigned long long bigs = ((unsigned long long)biguv) << (LONG_NBIT-1);
      unsigned long smalluv = small.v & LONG_NOSIGN_BIT_MASK;
      unsigned long long smalls = ((unsigned long long)smalluv) << (LONG_NBIT-1-drift);
      //handle signs
      unsigned long long r;
      if ( (big.v & LONG_SIGN_BIT_MASK)== (small.v & LONG_SIGN_BIT_MASK))
	r = bigs + smalls;
      else
	r = bigs - smalls; //no sign problem

      int pos = LONGLONG_NBIT - 1;
      while (pos && (!(r & (1LL<<pos))))
	pos--;
      //now pos is shift of first 1 bit
      unsigned long result = (r>> (pos-LONG_NBIT-1));
      //restore sign bit
      UFFloat res;
      res.v = result |  (big.v & LONG_SIGN_BIT_MASK);
      res.e = big.e - (LONG_NBIT-1)+(pos-LONG_NBIT-1);
      return res;
    }

    UFFloat operator -(const UFFloat &b) const
    {
      UFFloat big,small;
      if (e<b.e)
	{
	  small = *this;
	  big = b;
	}
      else if (e>b.e)
	{
	  small = b;
	  big = *this;
	}
      else if (abs(v)>abs(b.v))
	{
	  //we need exact absolute value ordering
	  small = b;
	  big = *this;
	}
      else
	{
	  small = *this;
	  big = b;
	}

      long drift = big.e-small.e;
      if (drift>= LONG_NBIT)
	return big;
      unsigned long biguv = big.v & LONG_NOSIGN_BIT_MASK;
      unsigned long long bigs = ((unsigned long long)biguv) << (LONG_NBIT-1);
      unsigned long smalluv = small.v & LONG_NOSIGN_BIT_MASK;
      unsigned long long smalls = ((unsigned long long)smalluv) << (LONG_NBIT-1-drift);
      //handle signs
      unsigned long long r;
      if ( (big.v & LONG_SIGN_BIT_MASK)== (small.v & LONG_SIGN_BIT_MASK))
	r = bigs - smalls;
      else
	r = bigs + smalls; //no sign problem

      int pos = LONGLONG_NBIT - 1;
      while (pos && (!(r & (1LL<<pos))))
	pos--;
      //now pos is shift of first 1 bit
      unsigned long result = (r>> (pos-LONG_NBIT-1));
      //restore sign bit
      UFFloat res;
      res.v = result |  (big.v & LONG_SIGN_BIT_MASK);
      res.e = big.e - (LONG_NBIT-1)+(pos-LONG_NBIT-1);
      return res;
    }


    friend std::ostream& operator <<(std::ostream &s, UFFloat u);
  private:
    long v;
    long e;
  };

  std::ostream& operator <<(std::ostream &s, UFFloat u);
  std::istream& operator >>(std::istream &s, UFFloat &u);

}

#endif // !LIBPORT_UFFLOAT_HH
