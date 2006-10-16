#
# $Id$
# Copyright (c) Gostai S.A.S., 2006.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com

## ------------- ##
## Package setup ##
## ------------- ##

AC_INIT([Program Name], [0.0.1], [some_mail@gostai.com], [tarball-name])

AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([foreign -Wall])

## -------------------- ##
## Tool/Compilers setup ##
## -------------------- ##

# Look for a C++ compiler.
URBI_PROG_CXX

# If you need to create a library, USE libtool:
AC_PROG_LIBTOOL

# If you happen to have some C code:
#AC_LANG([C])
#AC_PROG_CC

# AutoTroll with Qt.
AT_WITH_QT

AC_CONFIG_FILES([
  Makefile
    src/Makefile
    gui/Makefile
    doc/Makefile
    tests/Makefile
])

AC_CONFIG_FILES([doc/doxygen.sh], [chmod a=rx doc/doxygen.sh])

AC_OUTPUT
