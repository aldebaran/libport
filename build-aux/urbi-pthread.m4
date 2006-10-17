#						-*- Autoconf -*-

# URBI_PTHREAD
# ------------
# Look for the pthreads, or for windows.  Define the automake conditional
# WIN32.
AC_DEFUN([URBI_PTHREAD],
[ACX_PTHREAD([pthreads=true], [pthreads=false])
AC_CHECK_HEADERS([windows.h], [windows=true], [windows=false])
AC_CHECK_HEADERS([pthread.h])

if $windows; then
   AC_SUBST([PTHREAD_LIBS], ['-lws2_32 -lgdi32'])
fi
AM_CONDITIONAL([WIN32], [$windows])

#if $windows || $pthreads; then :; else
#  AC_MSG_ERROR([pthreads or WIN32 API is required])
#fi
])
