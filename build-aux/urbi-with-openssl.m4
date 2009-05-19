#
# urbi-with-openssl.m4: This file is part of build-aux.
# Copyright (C) 2006-2008, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

# URBI_OPENSSL
# ----------
# Look for the OpenSSL.  Support --with-openssl=PATH.  Check the library
# is there.
#
# Output variable OPENSSL_PATH point to the installation location.
# Set "$openssl" to "true" or "false".

AC_DEFUN([URBI_OPENSSL_CHECK],
[
found_ssl=false
saved_LIBS="$LIBS"
saved_LDFLAGS="$LDFLAGS"
saved_CPPFLAGS="$CPPFLAGS"

case $host_os in
     mingw*)
	LIBS="$LIBS -leay32"
	if test -n "$ssldir"; then
	   LDFLAGS="$LDFLAGS -L$ssldir"
	fi
	;;
     *cygwin*)
	LIBS="$LIBS -leay32MT"
	if test -n "$ssldir"; then
	   LDFLAGS="$LDFLAGS -L$ssldir"
	fi
	;;
     *)
	LIBS="$LIBS -lcrypto"
	if test -n "$ssldir"; then
	   LDFLAGS="$LDFLAGS -L$ssldir"
	else
	   LDFLAGS="$LDFLAGS -L/usr/lib"
	fi
	;;
esac


# Basic test about OpenSSL
AC_LINK_IFELSE(
[
AC_LANG_PROGRAM([[
	#include <string.h>
	#include <openssl/rand.h>
]],
[[      char a[2048];
	memset(a, 0, sizeof(a));
	return(0);
	RAND_add(a, sizeof(a), sizeof(a));
]])
],
[
	found_ssl=true
], [])

LIBS="$saved_LIBS"
LDFLAGS="$saved_LDFLAGS"
CPPFLAGS="$saved_CPPFLAGS"
])

AC_DEFUN([URBI_WITH_OPENSSL],
[AC_ARG_WITH([openssl],
	     [AC_HELP_STRING([--with-openssl=openssl-path],
			     [turn on OpenSSL [/usr/lib]])],
	     [], [with_openssl=yes])


saved_LIBS="$LIBS"
saved_LDFLAGS="$LDFLAGS"
saved_CPPFLAGS="$CPPFLAGS"

case $with_openssl in
   no) openssl=false
       ;;

  yes) openssl=true
       AC_CACHE_CHECK([for OpenSSL directory], ac_cv_openssldir, [
	for ssldir in "" /usr/lib /lib /usr/local/openssl /usr/lib/openssl \
			 /usr/local/ssl /usr/lib/ssl /usr/local /usr/pkg \
			 /opt /opt/openssl; do
		# Skip directories if they don't exist
		if test ! -z "$ssldir" && test ! -d "$ssldir" ; then
		   continue;
		fi
		# Check the library
		URBI_OPENSSL_CHECK
		# If it succeed, exit.
		if test "x$found_ssl" = "xtrue"; then
		   break;
		fi
	done

	case $found_ssl in
	  '' | false )
	   AC_MSG_ERROR(
  [Could not find working OpenSSL library, please install or check config.log])
	   ssldir=""
	  ;;
	  * )
	   if test -z "$ssldir"; then
	      ssldir="(system)"
	   fi
	  ;;
	esac

	ac_cv_openssldir=$ssldir;
       ])
       if test ! -z "$ac_cv_openssldir"; then
	   openssl=true
	   OPENSSL_PATH=$ac_cv_openssldir
       else
	   openssl=false
       fi
       ;;

    # In this case, no test is done.
    *) openssl=true
	# Define the path.
	ssldir=$with_openssl
	# Check the library.
	URBI_OPENSSL_CHECK
	# If it succeed, exit.
	if test "x$found_ssl" = "xtrue"; then
	  OPENSSL_PATH=$with_openssl
	else
	  AC_MSG_ERROR([I didn't succeed to compile with this library. Please check the path and the config.log])
	fi
       ;;
esac

if $openssl; then
  # More tools for OpenSSL.
  AC_SUBST([OPENSSL_CPPFLAGS],[""])
  case $OPENSSL_PATH:$host_os in
    '(system)':mingw*)   OPENSSL_LDFLAGS="-leay32";;
    '(system)':*cygwin*) OPENSSL_LDFLAGS="-leay32MT";;
    '(system)':*)        OPENSSL_LDFLAGS="-lcrypto -lssl -ldl";;
    *:mingw*)            OPENSSL_LDFLAGS="-L$OPENSSL_PATH/lib -leay32";;
    *:*cygwin*)          OPENSSL_LDFLAGS="-L$OPENSSL_PATH/lib -leay32MT";;
    *:*)                 OPENSSL_LDFLAGS="$OPENSSL_PATH/lib/libcrypto.la $OPENSSL_PATH/lib/libssl.la -ldl";;
  esac
  AC_SUBST([OPENSSL_LDFLAGS])
fi

AM_CONDITIONAL([OPENSSL], [$openssl])
AC_SUBST([OPENSSL_PATH])
])


AC_DEFUN([URBI_WITH_OPENSSL_REQUIRED],
[
   AC_REQUIRE([URBI_WITH_OPENSSL])

   case $openssl in
     '' | false )
      AC_MSG_ERROR(
   [Failed to find the OpenSSL library. Please install it or check config.log])
     ;;
   esac
])

## Local Variables:
## mode: autoconf
## End:
