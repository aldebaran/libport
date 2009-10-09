#
# urbi-module.m4: This file is part of build-aux.
# Copyright (C) 2006-2008, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

# This set of macro can be used to make a modular build system.
# Usage:
# * Include the .ac files of your modules, that must call
# URBI_MODULE(name, code). If code sets urbi_enable_module to false
# or if the user did not include the module in its list, the module will
# be disabled. BUILD_$name will be set in your .am files accordingly.
# * Call URBI_MODULAR_BUILD and URBI_MODULE_REPORT.


# URBI_MODULE_(name)
# ------------
# Add module to our list, and set default enable state based on
# module list given by the user
AC_DEFUN([URBI_MODULE_],
[
m4_append([URBI_modules], [$1], [,])dnl

  case ",$enable_modules," in
    (*,$1,*|,,) AM_CONDITIONAL([BUILD_$1], [true]) ;;
    (*)  URBI_MODULE_CONDITIONAL([$1], [false]) ;;
  esac
  eval urbi_desc_$1="$2"
])


# URBI_MODULE(name, code)
# -----------------------
# Regsister module.
# If the module is not allready disabled, call code, disable module if
# urbi_enable_module is false.
AC_DEFUN([URBI_MODULE],
[
URBI_MODULE_([$1])
if $urbi_module_$1_enabled; then
  urbi_enable_module=true
  $2
  URBI_MODULE_CONDITIONAL($1, $urbi_enable_module)
fi

])


# URBI_MODULE_CONDITIONAL(name, status)
# -------------------------------------
# Set AM_CONDITIONAL for this module, and add it
# to one of our enabled/disabled module list.
AC_DEFUN([URBI_MODULE_CONDITIONAL],
[
  AM_CONDITIONAL([BUILD_$1], [$2])
  urbi_module_$1_enabled=$2
  if $2; then
    urbi_enabled_modules="$urbi_enabled_modules $1"
  else
    urbi_disabled_modules="$urbi_disabled_modules $1"
  fi
])

# URBI_MODULE_REPORT
# ------------------
# Display the lists of enabled and disabled modules.
AC_DEFUN([URBI_MODULE_REPORT],
[
  AC_MSG_NOTICE([ The following modules will be built: $urbi_enabled_modules])
  AC_MSG_NOTICE([ The following modules will *not* be built: $urbi_disabled_modules])
])

# URBI_MODULAR_BUILD
# ------------------
# Add a --enable-modules=list configure option.
AC_DEFUN([URBI_MODULAR_BUILD],
[
  URBI_ARG_ENABLE([enable-modules],
    [ Comma-separated list of modules to enable from:],
    [.*],
    m4_defn([URBI_modules]),
    []
    )
])
