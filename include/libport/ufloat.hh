/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
/// \file libport/ufloat.hh
/// \brief Definition of the ufloat classes.

#ifndef LIBPORT_UFLOAT_HH
# define LIBPORT_UFLOAT_HH

# include <libport/config.h>
# include <libport/export.hh>

# include <cfloat>
# include <exception>
# include <typeinfo> // bad_cast

/*-----------------.
| Ufloat support.  |
`-----------------*/

# ifdef LIBPORT_URBI_UFLOAT_FLOAT
namespace libport
{
  typedef float ufloat;
}
# endif

# ifdef LIBPORT_URBI_UFLOAT_DOUBLE
namespace libport
{
  typedef double ufloat;
#  define UFLT_EPSILON DBL_EPSILON
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


/*---------------------.
| Float to int casts.  |
`---------------------*/

namespace libport
{
  class LIBPORT_API bad_numeric_cast : public std::bad_cast
  {
  public:
    virtual const char *what() const throw();
  };

  class LIBPORT_API negative_overflow : public bad_numeric_cast
  {
  public:
    virtual const char *what() const throw();
  };

  class LIBPORT_API positive_overflow : public bad_numeric_cast
  {
  public:
    virtual const char *what() const throw();
  };

  // numeric_cast must be hidden in the library as it uses Boost.
  //
  // Exporting template specializations does not seem to work the same
  // way bw MSVC and GCC.  With GCC it suffices to export the template
  // itself.  With MSVC we need to export explicitly all the
  // instantiations, but not the template.  Weirdly enough MSVC does
  // not see an explicit instantiation in this case, but truly a
  // declaration (adding extern does not seem to be useful).  But GCC
  // sees an explicit instantiation in this case, which is logical,
  // but of course fails (the true instantiation is in ufloat.cc, and
  // anyway the template definition is there too).
  //
  // So I could not find something to please both.

  // Also used in libport/ufloat.cc.
# define UFLOAT_CASTS                           \
                                                \
  UFLOAT_CAST(char);                            \
  UFLOAT_CAST(unsigned char);                   \
  UFLOAT_CAST(short);                           \
  UFLOAT_CAST(unsigned short);                  \
  UFLOAT_CAST(int);                             \
  UFLOAT_CAST(unsigned int);                    \
  UFLOAT_CAST(long);                            \
  UFLOAT_CAST(unsigned long);                   \
  UFLOAT_CAST(long long);                       \
  UFLOAT_CAST(unsigned long long);

# ifdef _MSC_VER

  /// Convert a libport::ufloat to T. Raise
  /// libport::bad_numeric_cast if the provided argument is not
  /// directly convertible to a T.
  template <typename T>
  T numeric_cast(ufloat v) throw (bad_numeric_cast);

#  define UFLOAT_CAST(Type)                                     \
  template                                                      \
  Type                                                          \
  LIBPORT_API                                                   \
  numeric_cast<Type>(ufloat v) throw (bad_numeric_cast);

  // Declare the casts.
  UFLOAT_CASTS

#  undef UFLOAT_CAST

# else // ! _MSC_VER

  /// Convert a libport::ufloat to T. Raise
  /// libport::bad_numeric_cast if the provided argument is not
  /// directly convertible to a T.
  template <typename T>
  LIBPORT_API
  T numeric_cast(ufloat v) throw (bad_numeric_cast);

# endif

} // namespace libport

# include <libport/ufloat.hxx>

#endif // !LIBPORT_UFLOAT_HH
