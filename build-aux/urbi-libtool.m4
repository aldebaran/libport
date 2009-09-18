#                                               -*- autoconf -*-
# urbi-libtool.m4: This file is part of build-aux.
# Copyright (C) 2008, 2009, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

# Don't call before the creation of libtool.  There is no means to
# control the order in which AC_CONFIG_COMMANDS are run, so just call
# this macro after the invocation to LT_INIT.
AC_DEFUN([URBI_LIBTOOL],
[# If you need to create a library, use libtool.
AC_REQUIRE([URBI_WIN32])dnl
LT_PREREQ([2.2.6])
LT_INIT([pic-only shared disable-static dlopen win32-dll])
AC_SUBST([LIBTOOL_DEPS])

if $windows; then
  # It is perfectly possible to link with static libraries when building dlls.
  # Also for some reason archive_cmds ends up empty for C++ tag.
  # It wants to use cygpath, but we are using wine, and anyway our
  # path are correct.
  AC_CONFIG_COMMANDS([libtool-quirk],
  [sed -i -r                                                    \
       -e 's/^archive_cmds=""/#&/'                              \
       -e 's/^(deplibs_check_method)=.*/#&\n\1=pass_all/'       \
       -e 's/^fix_srcfile_path="..*"/#&/'                       \
       libtool])
fi

case $host_alias in
     (*mingw*)
       # Let the compiler handle the standard libraries itself
       AC_CONFIG_COMMANDS([mingw-hacks],
           [sed -i -r                           \
               -e 's/^predep_objects=/#&/'      \
               -e 's/^postdeps=/#&/'            \
               -e 's/-nostdlib//g'              \
            libtool])
     ;;
esac
])
