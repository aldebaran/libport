#
# urbi-win32.m4: This file is part of build-aux.
# Copyright (C) 2006-2008, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

# URBI_WIN32
# ----------
# Look for windows. Define the automake conditional WIN32 as well
# as $windows.
AC_DEFUN([URBI_WIN32],
[AC_CHECK_HEADERS([windows.h], [windows=true], [windows=false])
AM_CONDITIONAL([WIN32], [$windows])

case $host_alias in
     (*mingw*)
       # Remove windows-style paths from dependencies files
       AC_CONFIG_COMMANDS([mingw-fix-path],
           [sed -i \
              "/mv.*Pl\?o$/{;h;s/$/ \\&\\&\\\\/;p;g;s,.*\.Tpo \(.*\),	sed -i s/c:\\\\\\\\\\\\\\\\/\\\\\\\\\\\\\\\\/g \1,;};" \
              Makefile
            ])
     ;;
esac
])

