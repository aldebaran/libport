/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/numeric/conversion/converter.hpp>

#include <libport/cmath>
#include <libport/cstdlib>
#include <libport/ufloat.hh>

#ifdef LIBPORT_URBI_UFLOAT_LONG_LONG
# include <libport/ull-fixed-point.cc>
#endif

#ifdef LIBPORT_URBI_UFLOAT_FLOATING
# include <libport/uffloat.cc>
#endif


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


  /*------------------------.
  | From string to ufloat.  |
  `------------------------*/

  // MSVC makes it uselessly complex to pass isdigit-like functions by
  // pointer.
  /// \param xdigit  whether hexadecimal instead of decimal.
  static inline
  bool
  is_digit(int c, bool xdigit)
  {
    return xdigit ? isxdigit(c) : isdigit(c);
  }

  /// Remove underscores, allowed only between two digits.
  ///
  /// \param xdigit  whether hexadecimal instead of decimal.
  static
  std::string
  as_ufloat_strip(const std::string& s, bool xdigit)
  {
    std::string res;
    size_t len = s.size();
    for (size_t i = 0; i < len; /* nothing */)
      if (s[i] == '_')
      {
        // Look for the next non-underscore character.
        size_t next = s.find_first_not_of("_", i);
        if (res.empty()
            || !is_digit(res[res.size() - 1], xdigit)
            || !(next < len)
            || !is_digit(s[next], xdigit))
          throw boost::bad_lexical_cast();
        i = next;
      }
      else
      {
        res.append(1, s[i]);
        ++i;
      }
    return res;
  }

  ufloat
  as_ufloat(const std::string& s)
  {
    // Convert.
    if (s.substr(0, 2) == "0x")
    {
      std::string pure = as_ufloat_strip(s, true);
      char* end = 0;
      ufloat res = strtoll(pure.c_str(), &end, 0);
      // Refuse garbage after.
      if (end && *end)
        throw boost::bad_lexical_cast();
      return res;
    }
    else
      return boost::lexical_cast<libport::ufloat>
        (as_ufloat_strip(s, false));
  }


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
      round_style;
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
