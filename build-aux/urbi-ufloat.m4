#
# urbi-ufloat.m4: This file is part of build-aux.
# Copyright (C) 2006-2008, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

m4_define([_URBI_UFLOAT_DEFINE],
[AC_DEFINE([URBI_UFLOAT_$1], [1], [Define if ufloats should be $2.])])



# URBI_UFLOAT
# -----------
# Select the variety of ufloat we use, and whether tabulated or not.
AC_DEFUN([URBI_UFLOAT],
[# Whether we use special ufloats or not.
AC_ARG_ENABLE([ufloat],
[  --enable-ufloat=KIND,TABULATED    the kind of ufloat to use.
				      [[double,not-tabulated]]
     KIND:
       - float, double: the C++ type
       - long, long long: fixed point implementation with longs
       - floating: floating with a long mantissa, and a long exponent
     TABULATED:
       - "tabulated"
       - "not-tabulated".],
  [], [enable_ufloat=double,not-tabulated])

AC_MSG_CHECKING([the kind of ufloats])
save_IFS=$IFS
IFS=,
for i in $enable_ufloat
do
  IFS=$save_IFS
  case $i in
    notab | nottab | not-tab | \
      "not tabulated" | nottabulated | not-tabulated )
      urbi_ufloat_tabulated="not tabulated";;
    tab   | tabulated    )
      urbi_ufloat_tabulated=tabulated;;
    float | double | "long" | "long long" | floating )
      urbi_ufloat_support=$i;;
    quad | long-long )
      urbi_ufloat_support="long long";;
    *)
      AC_MSG_ERROR([invalid ufloat specifier: $i]);;
  esac
done
AC_MSG_RESULT([$urbi_ufloat_tabulated $urbi_ufloat_support])

# Define the needed symbols.
case $urbi_ufloat_support in
  float)       _URBI_UFLOAT_DEFINE([FLOAT], [floats]);;
  double)      _URBI_UFLOAT_DEFINE([DOUBLE], [doubles]);;
  long)        _URBI_UFLOAT_DEFINE([LONG], [long]);;
  "long long") _URBI_UFLOAT_DEFINE([LONG_LONG], [long long]);;
  floating)    _URBI_UFLOAT_DEFINE([FLOATING], [long mantissa, long exponent]);;
  *)    AC_MSG_ERROR([internal error: invalid support: $urbi_ufloat_support]);;
esac

case $urbi_ufloat_tabulated in
  tabulated)   _URBI_UFLOAT_DEFINE([TABULATED], [tabulated]);;
  "not tabulated") ;;
  *)  AC_MSG_ERROR([internal error: invalid tabulation: $urbi_ufloat_tabulated]);;
esac
])


## Local Variables:
## mode: autoconf
## End:
