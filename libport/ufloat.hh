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

# include "config.h"

/*-----------------.
| Ufloat support.  |
`-----------------*/

# ifdef URBI_UFLOAT_FLOAT
namespace urbi
{
  typedef float ufloat;
}
# endif

# ifdef URBI_UFLOAT_DOUBLE
namespace urbi
{
  typedef double ufloat;
}
# endif

# ifdef URBI_UFLOAT_LONG
#  include "libport/ulong-fixed-point.hh"
# endif

# ifdef URBI_UFLOAT_LONG_LONG
#  include "libport/ull-fixed-point.hh"
# endif

# ifdef URBI_UFLOAT_FLOATING
#  include "libport/uffloat.hh"
# endif


namespace urbi
{
  static const ufloat PI(3.14159265358979323846264338327950288);
  static const ufloat UINFINITY(999999999999999.0);
}


/*-------------------.
| Ufloat tabulated.  |
`-------------------*/

# ifdef URBI_UFLOAT_TABULATED
namespace urbi
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


#endif
