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

CPPFLAGS="$CPPFLAGS -I/usr/include"

LIBS="$LIBS -lcrypto"
if test -n "$ssldir"; then
   LDFLAGS="$LDFLAGS -L$ssldir"
else
  LDFLAGS="$LDFLAGS -L/usr/lib"
fi

# Basic test about OpenSSL
AC_TRY_RUN(
[
#include <string.h>
#include <openssl/rand.h>
int main(void)
{
	char a[2048];
	memset(a, 0, sizeof(a));
	RAND_add(a, sizeof(a), sizeof(a));
	return(RAND_status() <= 0);
}
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

	case "$found_ssl" in
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
  if test "x(system)" = "x$OPENSSL_PATH"; then
    AC_SUBST([OPENSSL_LDFLAGS],["-lcrypto"])
  else
    AC_SUBST([OPENSSL_LDFLAGS],["-lcrypto -L$OPENSSL_PATH"])
  fi
fi

AM_CONDITIONAL([OPENSSL], [$openssl])
AC_SUBST([OPENSSL_PATH])
])


AC_DEFUN([URBI_WITH_OPENSSL_REQUIRED], [
   AC_REQUIRE([URBI_WITH_OPENSSL])

   case "$openssl" in
     '' | false )
      AC_MSG_ERROR(
   [Failed to find the OpenSSL library. Please install it or check config.log])
     ;;
   esac
])

## Local Variables:
## mode: autoconf
## End:
