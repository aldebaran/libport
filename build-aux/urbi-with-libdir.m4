#							-*- Autoconf -*-
# Copyright (C) 2006, 2007 Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com

# URBI_WITH_LIBDIR(OPTION, VARIABLE, DOC, LIBNAME)
#           e.g., ([urbi-kernel], [URBI_KERNEL_PATH], [URBI Kernel], [kernel])
# ----------------------------------------------------------------------------
# Adds an option --with-OPTION=PATH to configure.
# This option is mandatory. configure will check that it can find
# libLIBNAME.la at the path indicated. The path will be AC_SUBST'ed
# in $(VARIABLE).
AC_DEFUN([URBI_WITH_LIBDIR],
[m4_ifndef([DEFAULT_URBI_ENV],
	   [m4_fatal(DEFAULT_URBI_ENV is not defined.)])

  AC_ARG_WITH([$1],
    [AS_HELP_STRING([--with-$1],
       [Path (--prefix) where the $3 was installed (mandatory)])],
    [$2=$withval], [$2='NONE'])

  # Check we're given something.
  if test x"$$2" = xNONE; then
    AC_MSG_ERROR([You must pass the install prefix of the $3 to --with-$1])
  fi

  # Check we're given an absolute path.
  case $$2 in
    [[\\/]* | ?:[\\/]* ]) # Absolute
       ;;
    *)
      AC_MSG_ERROR([--with-$1=$$2: not an absolute path])
      ;;
  esac

  # Check we're given a directory.
  if test ! -d "$$2"; then
    AC_MSG_ERROR([--with-$1=$$2: not a directory])
  fi

  # Compute the various dir variables.
  URBI_DIRS(m4_defn([DEFAULT_URBI_ENV]))

  # Check we're given the prefix to the directory holding the lib.
  AC_CACHE_CHECK([for lib$4.la], [urbi_cv_lib$4_la],
  [urbi_cv_lib$4_la=no
  echo "$as_me:$LINENO: Checking whether '$$4dir/lib$4.la' exists" \
    >&AS_MESSAGE_LOG_FD
  if test ! -f "$$4dir/lib$4.la"; then
    urbi_cv_lib$4_la=yes
  fi
  ])
  if test x"$urbi_cv_lib$4_la" = xno; then
    AC_MSG_ERROR([--with-$1=$$2: $$4dir/lib$4.la is missing])
  fi

  AC_SUBST([$2])
  AC_SUBST([DISTCHECK_CONFIGURE_FLAGS],
	   ["$DISTCHECK_CONFIGURE_FLAGS --with-$1='$$2'"])
])


# URBI_WITH_KERNEL
# ----------------
AC_DEFUN([URBI_WITH_KERNEL],
[URBI_PACKAGE_KIND([core])
URBI_WITH_LIBDIR([urbi-kernel], [URBI_KERNEL_PATH], [URBI Kernel], [kernel])
])

# URBI_WITH_CORE
# --------------
## FIXME: We might not want to define URBI_KERNEL_PATH here.
AC_DEFUN([URBI_WITH_CORE],
[URBI_PACKAGE_KIND([core])
URBI_WITH_LIBDIR([urbi-core], [URBI_CORE_PATH], [URBI Core], [core])
AC_SUBST([URBI_KERNEL_PATH], [$URBI_CORE_PATH])
])
