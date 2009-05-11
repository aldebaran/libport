#                                               -*- autoconf -*-
# urbi-libtool.m4: This file is part of build-aux.
# Copyright (C) 2008, Gostai S.A.S.
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
LT_PREREQ([2.2.6])
LT_INIT([pic-only shared disable-static dlopen win32-dll])
AC_SUBST([LIBTOOL_DEPS])

case $CXX_FLAVOR in
  ('')
    AC_MSG_ERROR([CXX_FLAVOR undefined]);;
  (msvc)
    # It is perfectly possible to link with static libraries when building dlls.
    # Also for some reason archive_cmds ends up empty for C++ tag.
    AC_CONFIG_COMMANDS([libtool-quirk],
    [sed -i -r \
         -e 's/^archive_cmds=""/#& /' \
         -e 's/^deplibs_check_method=.*/#&\ndeplibs_check_method=pass_all/' \
         libtool])
 ;;
esac
])
