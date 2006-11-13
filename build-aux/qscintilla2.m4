#
# Copyright (C) Gostai S.A.S., 2006.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com

# This file relies on values (QT_*) defined by AutoTroll.
# Although it is not *required* to use AutoTroll along with this file, doing
# so provides better results since QScintilla is often located where Qt is.

# WITH_QSCINTILLA2()
# ------------------
AC_DEFUN([WITH_QSCINTILLA2],
[ AC_REQUIRE([AC_PROG_CXX])
  AC_REQUIRE([AC_EXEEXT])

  AC_ARG_WITH([qscintilla2],
              [AS_HELP_STRING([--with-qscintilla2].
                 [LDFLAGS to use for qscintilla2 @<:@-lqscintilla2 + Same as Qt@:>@])],
              [QSCINTILLA2_LDFLAGS=$withval], [QSCINTILLA2_LDFLAGS='-lqscintilla2'])

  # FIXME: Can we use a macro from autoconf to make something similar?
  AC_CACHE_CHECK([for lib qscintilla2], [qt_cv_lqscintilla2],
  [qt_cv_lqscintilla2=no
  cat >conftest.cpp <<_ASEOF
#include <qsciscintilla.h>
#ifdef main  // Temporary work-around for Windows: Qt #defines main qMain
# undef main // but for some reason *I* can't link Qt apps because the linker
#endif       // fails to find it. To be FIXME'd.
struct Test: public QsciScintilla
{
};

int main ()
{
  Test t;
}
_ASEOF
  # Dirty hack :(
  # Qt adds $(QT_DEFINES) in $QT_CXXFLAGS: we need to expand it here!
  eval_qt_defines_sed="s/\$(QT_DEFINES)/$QT_DEFINES/"
  MY_QT_CXXFLAGS=`echo "$QT_CXXFLAGS" | sed "$eval_qt_defines_sed"`

  echo "$as_me:$LINENO: running: $CXX $QT_CPPFLAGS $AM_CPPFLAGS $CPPFLAGS \
          $MY_QT_CXXFLAGS $AM_CXXFLAGS $CXXFLAGS \
          $QT_LDFLAGS $LDFLAGS \
          $QSCINTILLA2_LDFLAGS $QT_LIBS $LIBS conftest.cpp -o conftest$EXEEXT;" \
        >&AS_MESSAGE_LOG_FD
  if $CXX $QT_CPPFLAGS $AM_CPPFLAGS $CPPFLAGS \
          $MY_QT_CXXFLAGS $AM_CXXFLAGS $CXXFLAGS \
          $QT_LDFLAGS $LDFLAGS \
          $QSCINTILLA2_LDFLAGS $QT_LIBS $LIBS conftest.cpp -o conftest$EXEEXT \
       >&AS_MESSAGE_LOG_FD 2>&1;
  then
    if test x"$QT_LIBS" != x; then
      qt_cv_lqscintilla2="$QSCINTILLA2_LDFLAGS + Qt stuff"
    else
      qt_cv_lqscintilla2="$QSCINTILLA2_LDFLAGS"
    fi
  fi
  if test x"$qt_cv_lqscintilla2" = xno; then
    echo "$as_me:$LINENO: failed program was:" >&AS_MESSAGE_LOG_FD
    sed 's/^/| /' conftest.cpp >&AS_MESSAGE_LOG_FD
  fi
  rm -f conftest.cpp conftest$EXEEXT
  ])dnl End of AC_CACHE_CHECK for -lqscintilla2

  if test x"$qt_cv_lqscintilla2" = xno; then
    AC_MSG_ERROR([Cannot link with -lqscintilla2.])
  fi
  AC_SUBST([QSCINTILLA2_LDFLAGS])
])
