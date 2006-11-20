#
# Copyright (C) Gostai S.A.S., 2006.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com

# WITH_DISABLE_DEBUG()
# --------------------
# Adds a --disable-debug option to configure and add -DNDEBUG in the CPPFLAGS.
AC_DEFUN([WITH_DISABLE_DEBUG],
[AC_ARG_ENABLE([debug], [AS_HELP_STRING([--disable-debug],
                                       [Disable debugging for release mode.])]
               [],dnl action-if-given
               [AC_SUBST([AM_CPPFLAGS], ["$AM_CPPFLAGS -DNDEBUG"])
                AC_MSG_WARN([[Debugging disabled: No assert() will be performed.]])
                # FIXME: Remove -g from the default flags (if GXX is used etc.) ?
               ])
])
