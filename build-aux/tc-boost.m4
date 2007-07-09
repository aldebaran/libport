#                                                       -*- Autoconf -*-

# TC_HEADER_BOOST_1_32
# TC_HEADER_BOOST_1_34
# --------------------
# Wrappers around TC_HEADER_BOOST to use with AC_REQUIRE.
AC_DEFUN([TC_HEADER_BOOST_1_32],  [TC_HEADER_BOOST([1.32])])
AC_DEFUN([TC_HEADER_BOOST_1_34],  [TC_HEADER_BOOST([1.34])])



# TC_HEADER_BOOST(REQUIRED-VERSION)
# ---------------------------------
# Check for Boost headers version greater or equal than REQUIRED-VERSION.
AC_DEFUN([TC_HEADER_BOOST],
[ AC_ARG_WITH([boost],
	      [AC_HELP_STRING([--with-boost=DIR],
			      [location of Boost headers])])
  m4_if([$1], [],
	[AC_FATAL([This macro requires an argument])])dnl

  case $with_boost in
    '' | yes | no) ;;
		*)
      CPPFLAGS="-I$with_boost $CPPFLAGS"
      DISTCHECK_CONFIGURE_FLAGS="$DISTCHECK_CONFIGURE_FLAGS --with-boost=$with_boost"
      AC_SUBST([DISTCHECK_CONFIGURE_FLAGS])
      ;;
  esac

  AC_CHECK_HEADER([boost/version.hpp], [],
    [AC_MSG_ERROR([Boost headers are required.
      Use `--with-boost=DIR' or add `-I boost_include_path'
      to `CPPFLAGS'.])])

  # Some package manager (without naming apt) does not provide the
  # Boost Generic Graph headers in the main Boost package. So
  # `boost/version.hpp' can be present but not `boost/graph/*'
  AC_CHECK_HEADER([boost/graph/exception.hpp], [],
    [AC_MSG_ERROR([Boost Generic Graph headers are required.
      Use `--with-boost=DIR' or add `-I boost_include_path'
      to `CPPFLAGS'.])])

  tc_header_boost_major=`echo $1 | cut -d. -f1`
  tc_header_boost_minor=`echo $1 | cut -d. -f2`

  AC_CACHE_CHECK([for Boost headers >= $1],
    [ac_cv_boost_version],
    [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([#include "boost/version.hpp"
#if (BOOST_VERSION / 10000 < $tc_header_boost_major) \
   || (BOOST_VERSION / 100 % 1000 < $tc_header_boost_minor)
# error Boost headers < $tc_header_boost_major.$tc_header_boost_minor
#endif
])],
      [ac_cv_boost_version=yes],
      [ac_cv_boost_version=no])
    ])
  if test x$ac_cv_boost_version != xyes; then
    AC_MSG_ERROR([Boost headers >= $1 are required])
  fi
])
