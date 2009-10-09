#
# urbi-sockets.m4: This file is part of build-aux.
# Copyright (C) 2006-2008, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

# URBI_SOCKETS
# ------------
# Add the required library to use sockets.
AC_DEFUN([URBI_SOCKETS],
[AC_REQUIRE([URBI_WIN32])
if $windows; then
  AC_SUBST([SOCKETS_LIBS], ['-lws2_32 -lMsWSock'])
fi

SDK_LIBS="$SDK_LIBS $SOCKETS_LIBS"
])
