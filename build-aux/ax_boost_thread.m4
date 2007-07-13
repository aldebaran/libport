##### http://autoconf-archive.cryp.to/ax_boost_thread.html
#
# SYNOPSIS
#
#   AX_BOOST_THREAD
#
# DESCRIPTION
#
#   Test for Thread library from the Boost C++ libraries. The macro
#   requires a preceding call to AX_BOOST_BASE. Further documentation
#   is available at <http://randspringer.de/boost/index.html>.
#
#   This macro calls:
#
#     AC_SUBST(BOOST_THREAD_LIB)
#
#   And sets:
#
#     HAVE_BOOST_THREAD
#
# LAST MODIFICATION
#
#   2007-03-22
#
# COPYLEFT
#
#   Copyright (c) 2007 Thomas Porschberg <thomas@randspringer.de>
#   Copyright (c) 2007 Michael Tindal <mtindal@paradoxpoint.com>
#
#   Copying and distribution of this file, with or without
#   modification, are permitted in any medium without royalty provided
#   the copyright notice and this notice are preserved.

AC_DEFUN([AX_BOOST_THREAD],
[AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_CANONICAL_BUILD])dnl
AC_ARG_WITH([boost-thread],
	    [AS_HELP_STRING([--with-boost-thread@<:@=special-lib@:>@],
			    [use the Thread library from boost - it is]
			    [possible to specify a certain library for the]
			    [linker e.g.]
			    [--with-boost-thread=boost_thread-gcc-mt])],
	    [case $with_boost_thread in
	      no) ;;
	      yes)
		ax_boost_user_thread_lib=;;
	       *)
		ax_boost_user_thread_lib=$with_boost_thread
		$with_boost_thread=yes;;
	     esac],
	    [with_boost_thread=yes])

if test "x$with_boost_thread" = "xyes"; then
  CPPFLAGS_SAVED=$CPPFLAGS
  CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
  export CPPFLAGS

  LDFLAGS_SAVED=$LDFLAGS
  LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
  export LDFLAGS

  AC_CACHE_CHECK([whether the Boost::Thread library is available],
		 [ax_cv_boost_thread],
	[AC_LANG_PUSH([C++])
	 CXXFLAGS_SAVE=$CXXFLAGS
	 case $build_os in
	   solaris)  CXXFLAGS="-pthreads $CXXFLAGS";;
	   ming32)   CXXFLAGS="-mthreads $CXXFLAGS";;
	   *)        CXXFLAGS="-pthread  $CXXFLAGS";;
	 esac
	 AC_COMPILE_IFELSE(AC_LANG_PROGRAM([[@%:@include <boost/thread/thread.hpp>]],
					   [[boost::thread_group thrds;
					     return 0;]]),
			   [ax_cv_boost_thread=yes],
			   [ax_cv_boost_thread=no])
	CXXFLAGS=$CXXFLAGS_SAVE
	AC_LANG_POP([C++])
	])
  if test "x$ax_cv_boost_thread" = "xyes"; then
    # FIXME: This should be CXXFLAGS, not CPPFLAGS.
     case $build_os in
       solaris)  BOOST_CPPFLAGS="-pthreads $BOOST_CPPFLAGS";;
       ming32)   BOOST_CPPFLAGS="-mthreads $BOOST_CPPFLAGS";;
       *)        BOOST_CPPFLAGS="-pthread  $BOOST_CPPFLAGS";;
     esac
    AC_SUBST([BOOST_CPPFLAGS])
    AC_DEFINE([HAVE_BOOST_THREAD],,
	      [define if the Boost::Thread library is available])


    LDFLAGS_SAVE=$LDFLAGS
    case $build_os in
      *bsd* ) LDFLAGS="-pthread $LDFLAGS";;
    esac

    BN=boost_thread
    if test "x$ax_boost_user_thread_lib" = "x"; then
       for ax_lib in $BN $BN-mt $BN-$CC $BN-$CC-mt $BN-$CC-mt-s $BN-$CC-s \
		     lib$BN lib$BN-mt lib$BN-$CC lib$BN-$CC-mt \
			 lib$BN-$CC-mt-s lib$BN-$CC-s \
		     $BN-mgw $BN-mgw $BN-mgw-mt $BN-mgw-mt-s $BN-mgw-s
       do
	 AC_CHECK_LIB([$ax_lib], main,
		      [AC_SUBST([BOOST_THREAD_LIB], ["-l$ax_lib"])
		       link_thread="yes"; break],
		      [link_thread="no"])
       done
    else
       for ax_lib in $ax_boost_user_thread_lib $BN-$ax_boost_user_thread_lib
       do
	 AC_CHECK_LIB([$ax_lib], main,
		      [AC_SUBST([BOOST_THREAD_LIB], ["-l$ax_lib"])
		       link_thread="yes"; break],
		      [link_thread="no"])
       done
    fi

    if test "x$link_thread" = "xno"; then
       AC_MSG_ERROR([Could not link against $ax_lib!])
    else
       case $build_os in
	  *bsd* ) BOOST_LDFLAGS="-pthread $BOOST_LDFLAGS" ;;
       esac
    fi
  fi

  CPPFLAGS=$CPPFLAGS_SAVED
  LDFLAGS=$LDFLAGS_SAVED
fi
])

# Local Variables:
# mode: autoconf
# End:
