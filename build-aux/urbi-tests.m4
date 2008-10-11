#
# urbi-tests.m4: This file is part of build-aux.
# Copyright (C) Gostai S.A.S., 2006-2008.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

AC_PREREQ([2.60])

# URBI_CONFIG_WRAPPER COMPONENT-NAME
# ----------------------------------
# Create a testing wrapper for the COMPONENT-NAME.
m4_define([URBI_CONFIG_WRAPPER],
[AC_CONFIG_FILES([tests/$1:tests/wrapper.in], [chmod a+x tests/$1])
])

# URBI_TESTS
# ----------
# Set up the urbi test suite (in tests/).
AC_DEFUN([URBI_TESTS],
[# Prepare the umake wrappers.
URBI_CONFIG_WRAPPER([umake])
URBI_CONFIG_WRAPPER([umake-engine])
URBI_CONFIG_WRAPPER([umake-remote])
URBI_CONFIG_WRAPPER([umake-clean])
URBI_CONFIG_WRAPPER([umake-link])
URBI_CONFIG_WRAPPER([umake-lib])

# Prepare the Makefile.
AC_CONFIG_FILES([tests/Makefile])
])

## Local Variables:
## mode: autoconf
## End:
