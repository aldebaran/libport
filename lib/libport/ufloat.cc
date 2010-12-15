/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <iostream>
#include <libport/cmath>

#include <libport/ufloat.hh>

#ifdef LIBPORT_URBI_UFLOAT_LONG_LONG
# include <libport/ull-fixed-point.cc>
#endif

#ifdef LIBPORT_URBI_UFLOAT_FLOATING
# include <libport/uffloat.cc>
#endif

#include <boost/mpl/integral_c.hpp>
#include <boost/numeric/conversion/converter.hpp>

namespace libport
{
#ifdef LIBPORT_URBI_UFLOAT_TABULATED

# ifndef SINTABLE_POWER
  // The tables will containe 2^sintable_power elements.
#  define SINTABLE_POWER 10
# endif

  void buildSinusTable(int powersize);
  class DummyInit
  {
  public:
    DummyInit()
    {
      buildSinusTable(SINTABLE_POWER);
    }
  };

  DummyInit _dummyInit__;

  static ufloat* sinTable=0; //we store [O, PI/2[
  static ufloat* asinTable=0; //we store [O, 1[

  static unsigned long tableSize; //must be a power of two
  static int tableShift;

  void buildSinusTable(int powersize)
  {
    size_t size = 1<<powersize;
    tableShift = powersize;
    //don't use a step-based generation or errors will accumulate
    delete [] sinTable;
    delete [] asinTable;

    sinTable = new ufloat[size];
    asinTable = new ufloat[size];

    tableSize = size;
    for (size_t i=0;i<size;i++)
    {
      double idx = (double)i*(M_PI/2.0)/(double)size;
      float val = ::sin(idx);
      sinTable[i]=val;

      double idx2 = (double)i/(double)size;
      asinTable[i] = ::asin(idx2);
    }
  }

#endif


#if defined LIBPORT_URBI_UFLOAT_TABULATED \
 && !defined LIBPORT_URBI_UFLOAT_FLOATING

  ufloat tabulatedSin(ufloat val)
  {
    ufloat fidx = (val*(ufloat)tableSize / (M_PI/2.0));
    int idx = (int) fidx;
    ufloat rem = fidx -(ufloat)idx;

    idx = idx & (tableSize-1);

    if (fmod(val, M_PI) >= M_PI/2)
      idx = (tableSize-idx-1); //sin(pi/2+x) = sin(pi/2-x)
    ufloat interp = sinTable(idx)*(1.0-rem)+sinTable[(idx+1)%tableSize]*rem;

    return fmod(val, M_PI*2) > M_PI ? -interp : interp;
  }

  ufloat tabulatedCos(ufloat val)
  {
    ufloat fidx = (val*(ufloat)tableSize / (M_PI/2.0));
    int idx = (int) fidx;
    ufloat rem = fidx -(ufloat)idx;

    idx = idx & (tableSize-1);

    if (fmod(val, M_PI) < M_PI/2)
      idx = (tableSize-idx-1); //sin(pi/2+x) = sin(pi/2-x)

    ufloat interp = sinTable(idx)*(1.0-rem)+sinTable[(idx+1)%tableSize]*rem;

    return fmod(val, M_PI*2) > M_PI ? -interp : interp;
  }


  ufloat tabulatedASin(ufloat val)
  {
    ufloat fidx = val *(ufloat)tableSize;
    int idx =(int) fidx;
    ufloat rem = fidx -(ufloat)idx;
    idx = idx & (tableSize-1);
    ufloat interp = asinTable(idx)*(1.0-rem)+asinTable[(idx+1)%tableSize]*rem;

    return val < 0.0 ? -interp : interp;
  }

#endif

/*------------------.
| cast exceptions.  |
`------------------*/

  const char *bad_numeric_cast::what() const throw()
  {
    return "bad numeric conversion: overflow or non empty fractional part";
  }

  const char *negative_overflow::what() const throw()
  {
    return "bad numeric conversion: negative overflow";
  }

  const char *positive_overflow::what() const throw()
  {
    return "bad numeric conversion: positive overflow";
  }


/*-------------------.
| ufloat converter.  |
`-------------------*/

  template<typename S>
  struct ExactFloat2IntRounderPolicy
  {
    typedef S source_type;
    typedef S argument_type;

    static source_type nearbyint(argument_type s)
    {
      if (s != ceil(s))
	throw boost::numeric::bad_numeric_cast();
      return s;
    }

    typedef boost::mpl::integral_c<std::float_round_style,std::round_to_nearest>
      round_style ;
  };


  /*-------------------.
  | numeric_castable.  |
  `-------------------*/

  template <typename T>
  bool
  numeric_castable(ufloat val)
  {
    double int_part;
    if (modf(val, &int_part) != 0)
      return false;

    static boost::numeric::converter
      <T,
      ufloat,
      boost::numeric::conversion_traits<T, ufloat>,
      boost::numeric::def_overflow_handler,
      ExactFloat2IntRounderPolicy<ufloat> > converter;

    return converter.out_of_range(val) == boost::numeric::cInRange;
  }

# define UFLOAT_CAST(Type)                                      \
  template                                                      \
  bool numeric_castable<Type>(ufloat v);

  // Instantiate.
  UFLOAT_CASTS

#undef UFLOAT_CAST


  /*---------------.
  | numeric_cast.  |
  `---------------*/

  template <typename T>
  T
  numeric_cast(ufloat val) throw (bad_numeric_cast)
  {
    try
    {
      static boost::numeric::converter
        <T,
        ufloat,
        boost::numeric::conversion_traits<T, ufloat>,
        boost::numeric::def_overflow_handler,
        ExactFloat2IntRounderPolicy<ufloat> > converter;

      return converter(val);
    }
#define RETHROW(Name)                           \
    catch (boost::numeric::Name&)               \
    {                                           \
      throw Name();                             \
    }
    RETHROW(negative_overflow)
    RETHROW(positive_overflow)
    RETHROW(bad_numeric_cast)
#undef RETHROW
  }

# define UFLOAT_CAST(Type)                                      \
  template                                                      \
  Type                                                          \
  numeric_cast<Type>(ufloat v) throw (bad_numeric_cast);

  // Instantiate.
  UFLOAT_CASTS

#undef UFLOAT_CAST

} // namespace libport
