/*! \file ufloat.hh
 *******************************************************************************

 File: ufloat.hh\n
 Definition of the ufloat classes.

 This file is part of
 %URBI Kernel, version __kernelversion__\n
 (c) Jean-Christophe Baillie, 2004-2005.

 This software is provided "as is" without warranty of any kind,
 either expressed or implied, including but not limited to the
 implied warranties of fitness for a particular purpose.

 For more information, comments, bug reports: http://www.urbiforge.net

 **************************************************************************** */

#ifndef LIBPORT_UFLOAT_HH
# define LIBPORT_UFLOAT_HH

# include "libport/config.h"

# include <cfloat>

/*-----------------.
| Ufloat support.  |
`-----------------*/

# ifdef LIBPORT_URBI_UFLOAT_FLOAT
namespace libport
{
  typedef float ufloat;
#  define UFLT_EPSILON FLT_EPSILON

  inline
  long long to_long_long (ufloat u)
  {
    return static_cast <long long> (u);
  }
}
# endif

# ifdef LIBPORT_URBI_UFLOAT_DOUBLE
namespace libport
{
  typedef double ufloat;
#  define UFLT_EPSILON DBL_EPSILON

  inline
  long long to_long_long (ufloat u)
  {
    return static_cast <long long> (u);
  }
}
# endif

# ifdef LIBPORT_URBI_UFLOAT_LONG
#  include "libport/ulong-fixed-point.hh"
# endif

# ifdef LIBPORT_URBI_UFLOAT_LONG_LONG
#  include "libport/ull-fixed-point.hh"
# endif

# ifdef LIBPORT_URBI_UFLOAT_FLOATING
#  include "libport/uffloat.hh"
# endif


namespace libport
{
  static const ufloat PI(3.14159265358979323846264338327950288);
  static const ufloat UINFINITY(999999999999999.0);
}


/*-------------------.
| Ufloat tabulated.  |
`-------------------*/

# ifdef LIBPORT_URBI_UFLOAT_TABULATED
namespace libport
{
  /// return the tabulated sinus of given value in radian, using linear interpolation
  ufloat tabulatedSin(ufloat angle);
  /// return the tabulated cosinus of given value in radian, using linear interpolation
  ufloat tabulatedCos(ufloat angle);
  /// return the tabulated arcsinus of given value, in radian, using linear interpolation
  ufloat tabulatedASin(ufloat val);
  /// return the tabulated arccosinus of given value, in radian, using linear interpolation
  inline ufloat tabulatedACos(ufloat val) {return (PI>>1)-tabulatedASin(val);}

  inline ufloat sin(ufloat angle) {return tabulatedSin(angle);}
  inline ufloat cos(ufloat angle) {return tabulatedCos(angle);}
  inline ufloat tan(ufloat angle) {return sin(angle)/cos(angle);}

  inline ufloat asin(ufloat angle) {return tabulatedASin(angle);}
  inline ufloat acos(ufloat angle) {return tabulatedACos(angle);}
}
# endif

/* round is not C++ standard (not even POSIX) and neither gnulib nor Boost
 * provide one.  So here is my quick replacement.  */
# ifndef HAVE_ROUND
#  include <cmath>
namespace libport
{
  inline float round (float d)
  {
    return floor (d + 0.5 + FLT_EPSILON);
  }

  inline double round (double d)
  {
    return floor (d + 0.5 + DBL_EPSILON);
  }

  inline long double round (long double d)
  {
    return floor (d + 0.5 + LDBL_EPSILON);
  }
}
# endif /* !HAVE_ROUND */

#endif
