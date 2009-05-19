#
# m4-after.m4: This file is part of build-aux.
# Copyright (C) 2006-2008, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

AC_PREREQ([2.50])

# Fake definition to please aclocal.  Overriden below.
AC_DEFUN([m4_after],
[m4_fatal([this code must not run, it is here only to let aclocal
	  know that we define m4_after in this file
	  (we use m4_define, and aclocal looks only for AC_DEFUN)])dnl
])



# m4_after(THIS-MACRO-NAME, CALLED-MACRO-NAME)
# --------------------------------------------
# Error out if THIS-MACRO-NAME was not run before the CALLED-MACRO-NAME.
#
# Contrary to m4_require, it does not invoke the macro, it triggers
# an autoconf-time error.  Similar to m4_before.
#
# The macro *must* be defined with AC_DEFUN or m4_defun, m4_define won't
# do.
#
# This macros should be part of Autoconf.  And m4_before should also
# be equipped with the m4_defun'ed check.
m4_define([m4_after],
[m4_ifndef([m4_location($2)],
	   [m4_fatal([$0: $2 must be defined with m4_defun])])dnl
m4_provide_if([$2], [],
	      [m4_fatal([$2 must be invoked before $1])])dnl
])


# AC_AFTER(THIS-MACRO-NAME, CALLED-MACRO-NAME)
# --------------------------------------------
m4_copy([m4_after], [AC_AFTER])


## Local Variables:
## mode: autoconf
## End:
