# OPENSSL
# ----------
# Look for the OpenSSL.  Support --with-openssl=PATH.  Check the library
# is there.
#
# Output variable OPENSSL_PATH point to the installation location.
# Set "$openssl" to "true" or "false".

AC_DEFUN([OPENSSL_CHECK],
[
found_ssl=false
saved_LIBS="$LIBS"
saved_LDFLAGS="$LDFLAGS"
saved_CPPFLAGS="$CPPFLAGS"

LIBS="$saved_LIBS -lcrypto"
if test -z $ssldir; then
   LDFLAGS="$saved_LDFLAGS -L$ssldir"
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
],
[
	found_ssl=true
], [])

LIBS="$saved_LIBS"
LDFLAGS="$saved_LDFLAGS"
CPPFLAGS="$saved_CPPFLAGS"
])

AC_DEFUN([OPENSSL],
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
        for ssldir in "" /usr/local/openssl /usr/lib/openssl /usr/local/ssl \
			/usr/lib/ssl /usr/local /usr/pkg /opt /opt/openssl; do
		# Skip directories if they don't exist
		if test ! -z "$ssldir" -a ! -d "$ssldir" ; then
		   continue;
		fi
		# Check the library
		OPENSSL_CHECK
		# If it succeed, exit.
		if test ! -z "$found_ssl" ; then
		   break;
		fi
	done

	if test -z "$found_ssl"; then
	   AC_MSG_ERROR([Could not find working OpenSSL library, please install or check config.log])
	fi
	if test -z "$ssldir"; then
	   ssldir="(system)"
	fi

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
	OPENSSL_CHECK
	# If it succeed, exit.
	if test ! -z "$found_ssl" ; then
          OPENSSL_PATH=$with_openssl
	else
	  AC_MSG_ERROR([I didn't succeed to compile with this library. Please check the path and the configure.log])
	fi
       ;;
esac

if $openssl; then
  # More tools for OpenSSL.
  if test "(system)" = "$OPENSSL_PATH"; then
    AC_SUBST([OPENSSL_LDFLAGS],["-lcrypto"])
  else
    AC_SUBST([OPENSSL_LDFLAGS],["-lcrypto -L$OPENSSL_PATH"])
  fi
fi

AM_CONDITIONAL([OPENSSL], [$openssl])
AC_SUBST([OPENSSL_PATH])
])


AC_DEFUN([OPENSSL_REQUIRED], [
   AC_REQUIRE([OPENSSL])

   if test -z $openssl -o $openssl = false; then
      AC_MSG_ERROR([Failed to find the OpenSSL library. Please install it or check config.log])
   fi
])

## Local Variables:
## mode: autoconf
## End:
