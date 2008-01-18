##							-*- autoconf -*-
## urbi-kernel-version.m4: This file is part of build-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# URBI_KERNEL_VERSION
# -------------------
# This requires that you invoke AC_INIT with a version number (2nd argument)
# that matches the following ERE: ^\d+\.\d+(\.\d+)?
# Defines (AC_SUBST) the following variables:
#   KERNEL_VERSION, KERNEL_VERSION_MAJOR, KERNEL_VERSION_MINOR,
#   KERNEL_VERSION_SUBMINOR, KERNEL_REVISION
# and adds include/kernel/version.hh in AC_CONFIG_FILES
AC_DEFUN([URBI_KERNEL_VERSION],
[m4_define([KERNEL_VERSION_MAJOR_NUMBER],
[m4_bregexp(m4_defn([AC_PACKAGE_VERSION]), [^[0-9]+], [\&])])dnl
m4_define([KERNEL_VERSION_MINOR_NUMBER],
[m4_bregexp(m4_defn([AC_PACKAGE_VERSION]), [^[0-9]+\.\([0-9]+\)], [\1])])dnl
m4_define([KERNEL_VERSION_SUBMINOR_NUMBER],
[m4_default(m4_bregexp(m4_defn([AC_PACKAGE_VERSION]),
                       [^[0-9]+\.[0-9]+\.\([0-9]+\)], [\1]),
            [0])])dnl
dnl The version number of the form XYZ
m4_define([KERNEL_VERSION_NUMBER],
[m4_eval(KERNEL_VERSION_MAJOR_NUMBER * 100000
         + KERNEL_VERSION_MINOR_NUMBER * 100
         + KERNEL_VERSION_SUBMINOR_NUMBER)])dnl
AC_SUBST([KERNEL_VERSION], [KERNEL_VERSION_NUMBER])
AC_SUBST([KERNEL_VERSION_MAJOR], [KERNEL_VERSION_MAJOR_NUMBER])
AC_SUBST([KERNEL_VERSION_MINOR], [KERNEL_VERSION_MINOR_NUMBER])
AC_SUBST([KERNEL_VERSION_SUBMINOR], [KERNEL_VERSION_SUBMINOR_NUMBER])
AC_SUBST([KERNEL_REVISION],
[`if test -d "$srcdir/.git"; then
  env "GIT_DIR=$srcdir/.git" \
  git rev-list --pretty='format:%h' HEAD --max-count=1 \
    | sed '1d;s/^/g/;q'
elif test -d "$srcdir/.svn"; then
  svn info "$srcdir" | sed '/^Revision: /!d;s///'
else
  echo unknown
fi`])
AC_CONFIG_FILES([include/kernel/version.hh])
])
