#
# Copyright (c) Gostai S.A.S., 2008.
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

AC_INIT([Libport], [1.0], [common-bugs@lists.gostai.com])

AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([foreign -Wall])

## -------------------- ##
## Tool/Compilers setup ##
## -------------------- ##

# Look for a C++ compiler.
URBI_PROG_CXX

AC_CONFIG_HEADERS([include/config.h])

# If you need to create a library, USE libtool:
AC_PROG_LIBTOOL

# AutoTroll with Qt.
# AT_WITH_QT
# AC_CONFIG_FILES([gui/Makefile])

## -------------- ##
## Sub packages.  ##
## -------------- ##

URBI_CRYPTO

# Basically, the following lines states we install in prefix.
URBI_PACKAGE_KIND([sdk])
URBI_LIBPORT

AC_CONFIG_FILES([
  Makefile
      lib/libport/Makefile
      lib/urbi-crypto/Makefile
    tests/Makefile
      tests/libport/Makefile
      tests/urbi-crypto/Makefile
    doc/Makefile
])

AC_CONFIG_FILES([doc/doxygen.sh],    [chmod a=rx doc/doxygen.sh])
AC_CONFIG_FILES([build-aux/test.sh], [chmod a=rx build-aux/test.sh])

AC_OUTPUT
