/// \file libport/ufloat.hh
/// \brief Definition of the ufloat classes.

#ifndef LIBPORT_UFLOAT_HH
# define LIBPORT_UFLOAT_HH

# include <libport/config.h>
# include <libport/export.hh>

# include <cfloat>
# include <exception>

/*-----------------.
| Ufloat support.  |
`-----------------*/

# ifdef LIBPORT_URBI_UFLOAT_FLOAT
namespace libport
{
  typedef float ufloat;
#  define UFLT_EPSILON FLT_EPSILON

  inline long long to_long_long (ufloat u);
}
# endif

# ifdef LIBPORT_URBI_UFLOAT_DOUBLE
namespace libport
{
  typedef double ufloat;
#  define UFLT_EPSILON DBL_EPSILON

  inline long long to_long_long (ufloat u);
}
# endif

# ifdef LIBPORT_URBI_UFLOAT_LONG
#  include <libport/ulong-fixed-point.hh>
# endif

# ifdef LIBPORT_URBI_UFLOAT_LONG_LONG
#  include <libport/ull-fixed-point.hh>
# endif

# ifdef LIBPORT_URBI_UFLOAT_FLOATING
#  include <libport/uffloat.hh>
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
  inline ufloat tabulatedACos(ufloat angle);

  inline ufloat sin(ufloat angle);
  inline ufloat cos(ufloat angle);
  inline ufloat tan(ufloat angle);

  inline ufloat asin(ufloat angle);
  inline ufloat acos(ufloat angle);
}
# endif

# include <libport/config.h>
/* round is not C++ standard (not even POSIX) and neither gnulib nor Boost
 * provide one.  So here is my quick replacement.  */
# ifndef LIBPORT_HAVE_ROUND
#  include <cmath>
namespace libport
{
  inline float round (float d);
  inline double round (double d);
  inline long double round (long double d);

}
# endif /* !LIBPORT_HAVE_ROUND */

/* trunc is not C++ standard (not even POSIX) and although gnulib says it
 * provides a replacement in its manual, in fact it doesn't, nor does Boost.
 * So here is my quick replacement.  */
# ifndef LIBPORT_HAVE_TRUNC
#  include <cmath>
namespace libport
{
  LIBPORT_API inline double trunc (double d);
}
# endif /* !LIBPORT_HAVE_TRUNC */

/* Float to int converter.  */
namespace libport
{
  struct LIBPORT_API bad_numeric_cast : public std::exception {};

  /// Convert a libport::ufloat to a int. Raise
  /// libport::bad_numeric_cast if the provided argument is not
  /// directly convertible to an integer.
  LIBPORT_API int ufloat_to_int(ufloat val);
  LIBPORT_API long long ufloat_to_long_long(ufloat val);

} // namespace libport

# include <libport/ufloat.hxx>

#endif // !LIBPORT_UFLOAT_HH
