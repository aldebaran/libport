#                                                       -*- Autoconf -*-
#
# tc-prog.m4: This file is part of build-aux.
# Copyright (C) 2006-2008, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

# TC_PROG(PROGRAM, REQUIREMENT, VARIABLE, DESCRIPTION)
# ----------------------------------------------------
# Check whether PROGRAM version matches the REQUIREMENT.
# REQUIREMENT ::= OP VERS.
#          OP ::= < | <= | == | >= | > | !=
#        VERS ::= [\w.]*
#
# Define VARIABLE as an Autoconf variable for this program, along with
# its DESCRIPTION.
#
# The result is cached but with a name depending on VARIABLE, not PROGRAM,
# so that different requirement can be implemented for the same program.
AC_DEFUN([TC_PROG],
[AC_ARG_VAR([$3], [$4])
AC_CHECK_PROGS([$3], [$1])
# Alternation is not portable, thank you OSX...  So accept too
# many operators, and filter afterwards.
tc_re=['^\([<!=>=]*\) *\([0-9a-zA-Z_+.]*\)$']
tc_ver=`echo "$2" | sed "s/$tc_re/\2/"`
tc_req=`echo "$2" | sed "s/$tc_re/\1/"`
case $tc_req in
  "==" | "!=" | "<" | ">" | "<=" | ">=" ) ;;
  *) AC_MSG_ERROR([Invalid operator: $tc_req]);;
esac
if test -z "$tc_ver" || test -z "$tc_req"; then
  AC_MSG_ERROR([This macro requires an argument such as >= 2.2, != 2.3, etc.])
fi
if test -n "$$3" ; then
  AC_CACHE_CHECK([for $1 $tc_req $tc_ver],
    [ac_cv_$3_version],
    [ac_cv_$3_actual_version=`$$3 --version | \
	sed -n ['/^[^0-9]*\([0-9][0-9.]*[0-9][A-Za-z+]*\).*$/{s//\1/;p;q;}']`
    if test -z "$ac_cv_$3_actual_version"; then
	ac_cv_$3_version=no
    else
	ac_cv_$3_version=yes
    fi
    if test x$ac_cv_$3_version = xyes ; then
      AS_VERSION_COMPARE([$ac_cv_$3_actual_version], [$tc_ver],
			 [tc_cmp='<'],
			 [tc_cmp='='],
			 [tc_cmp='>'])
      case $tc_req:$tc_cmp in
	'<:<' | \
	'<=:<' | '<=:=' |\
	'==:=' | \
	'!=:<' | '!=:>' |\
	'>=:>' | '>=:=' |\
	'>:>')
	  ac_cv_$3_version=yes
	  ;;
	*)
	  ac_cv_$3_version=no
	  ;;
      esac
    fi])
fi
if test x$ac_cv_$3_version != xyes; then
  AC_MSG_ERROR([$1 $tc_req $tc_ver is required, have $ac_cv_$3_actual_version])
fi
])
