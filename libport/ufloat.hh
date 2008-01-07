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
  /** @return the tabulated sinus of given @a angle in radian, using linear
   * interpolation.  */
  ufloat tabulatedSin(ufloat angle);

  /** @return the tabulated cosinus of given @a angle in radian, using linear
   * interpolation.  */
  ufloat tabulatedCos(ufloat angle);

  /** @return the tabulated arcsinus of given @a angle, in radian, using
   * linear interpolation.  */
  ufloat tabulatedASin(ufloat angle);

  /** @return the tabulated arccosinus of given @a angle, in radian, using
   * linear interpolation.  */
  inline ufloat tabulatedACos(ufloat angle)
  {
    return (PI >> 1) - tabulatedASin(angle);
  }

  inline ufloat sin(ufloat angle) {return tabulatedSin(angle);}
  inline ufloat cos(ufloat angle) {return tabulatedCos(angle);}
  inline ufloat tan(ufloat angle) {return sin(angle)/cos(angle);}

  inline ufloat asin(ufloat angle) {return tabulatedASin(angle);}
  inline ufloat acos(ufloat angle) {return tabulatedACos(angle);}
}
# endif

# include "libport/config.h"
/* round is not C++ standard (not even POSIX) and neither gnulib nor Boost
 * provide one.  So here is my quick replacement.  */
# ifndef LIBPORT_HAVE_ROUND
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
# endif /* !LIBPORT_HAVE_ROUND */

/* trunc is not C++ standard (not even POSIX) and although gnulib says it
 * provides a replacement in its manual, in fact it doesn't, nor does Boost.
 * So here is my quick replacement.  */
# ifndef LIBPORT_HAVE_TRUNC
#  include <cmath>
namespace libport
{
  inline double trunc (double d)
  {
    if (d < 0.0)
      return ceil (d);
    else
      return floor (d);
  }
}
# endif /* !LIBPORT_HAVE_TRUNC */


#endif // !LIBPORT_UFLOAT_HH
