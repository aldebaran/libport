##
## urbi-qt-ext.m4: This file is part of build-aux.
## Copyright (C) 2007-2008, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# URBI_CONFIGURE_QT_EXT
# ---------------------
# Configure the qt-ext library.
AC_DEFUN([URBI_CONFIGURE_QT_EXT],
[
  AT_WITH_QT([])
  AT_REQUIRE_QT_VERSION([4.3])
  AC_CONFIG_SUBDIRS([qt-ext])
])

# URBI_WITH_QT_EXT
# ----------------
# Call this macro in your configrue.ac if you want to use qt-ext. Il will
# configure it.
AC_DEFUN([URBI_WITH_QT_EXT],
[
  URBI_CONFIGURE_QT_EXT
])
