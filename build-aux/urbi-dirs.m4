#
# urbi-dirs.m4: This file is part of build-aux.
# Copyright (C) Gostai S.A.S., 2006-2008.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

m4_pattern_forbid([^URBI_])

AC_PREREQ([2.60])

# URBI_DIRS(DEFAULT-URBI-ENV)
# ---------------------------
# See URBI_DIRS and URBI_KERNEL_DIRS below.
#
# DEFAULT-URBI-ENV should probably be something like "aibo", "webots"
# etc.  This macro *MUST* be invoked *AFTER* URBI_WITH_KERNEL
# (sdk/urbi-with-kernel.m4).
#
# Define the directory variables we are about to use:
#
# branddir	   = ${prefix}/${PACKAGE_BRAND}
#                    /usr/local/gostai
#
# kbranddir	   - Where the kernel is installed.
#                  = ${brandir} for kernels
#                  = ${URBI_KERNEL_PATH}/${PACKAGE_BRAND} for others
#                    /usr/local/gostai           via --with-urbi-kernel.
#
# urbiincludedir   - Where the UObject headers are installed
#                  = ${kernelincludedir}/urbi for kernels
#                    /usr/local/gostai/kernel/include/urbi
#                  = ${includedir}/urbi for others
#                    /usr/local/include/urbi
#
# hostdir	   = ${branddir}/KIND/${URBI_HOST}
#
# envdir	   - Where the kernel libraries are installed.
#                  = ${hostdir}/${URBI_ENV}
#                    /usr/local/gostai/core/aibo
#
# kerneldir	   = ${kbranddir}/kernel/${URBI_HOST}/${URBI_ENV}
#                    /usr/local/gostai/kernel/aibo
#
# sdkincludedir	   = ${branddir}/KIND/include
#
# kernelincludedir = ${kbranddir}/kernel/include
AC_DEFUN([URBI_DIRS],
[# We need to know the host (the type of architecture it will run on),
# the environment (the runtime, the event loop: engine, webots, aibo).

# URBI_HOST
AC_ARG_ENABLE([host],
	      [AC_HELP_STRING([--enable-host=urbi-host],
			      [The machine this will run on [HOST]])])
AC_REQUIRE([AC_CANONICAL_HOST])
AC_MSG_CHECKING([for URBI host type])
case $enable_host:$host_alias in
  ('':'') URBI_HOST=$host;;
  ('':* ) URBI_HOST=$host_alias;;
  ( *:* ) URBI_HOST=$enable_host;;
esac
AC_MSG_RESULT([$URBI_HOST])
AC_SUBST([URBI_HOST])
AC_DEFINE_UNQUOTED([URBI_HOST], ["$URBI_HOST"],
                   [Define as the Urbi host architecture name.])

# URBI_ENV
AC_ARG_ENABLE([env],
	      [AC_HELP_STRING([--enable-env=urbi-env],
			      [The environment this will run on:
			       aibo, webots, korebot, engine [$1]])])
AC_MSG_CHECKING([for URBI environment type])
case $enable_env in
 ('') URBI_ENV=$1;;
  (*) URBI_ENV=$enable_env;;
esac
AC_MSG_RESULT([$URBI_ENV])
case $URBI_ENV in
  (aibo)
     AC_DEFINE([URBI_ENV_AIBO], [1], [Define if compiling for Aibo.]);;
  (korebot)
     AC_DEFINE([URBI_ENV_KOREBOT], [1],
	       [Define if compiling for Korebot-based robots.]);;
  (engine)
     AC_DEFINE([URBI_ENV_ENGINE], [1], [Define if compiling generic engine.]);;
  (webots)
     AC_DEFINE([URBI_ENV_WEBOTS], [1], [Define if compiling for Webots.]);;
  (*)
     AC_MSG_NOTICE([[unknown environment type: $URBI_ENV]]);;
esac
AC_SUBST([URBI_ENV])

# URBI_ROOT.
AC_DEFINE_UNQUOTED([URBI_ROOT], ["$prefix"],
                   [Define as the install prefix.])

# Everything is installed in $URBI_KERNEL_PATH/gostai.
AC_SUBST([PACKAGE_BRAND], [gostai])
AC_SUBST([branddir], ['${prefix}/${PACKAGE_BRAND}'])
URBI_PACKAGE_KIND_SWITCH(
  [core],   [AC_SUBST([kbranddir], ['${URBI_KERNEL_PATH}/${PACKAGE_BRAND}'])],
  [kernel], [AC_SUBST([kbranddir], ['${branddir}'])])


# UObject headers
# ---------------
# The kernel installs them in its kernelincludedir.  The other
# package kinds install the standard headers and libraries in regular
# includedir and libdir under $prefix.  urbi-sdk headers are installed in
# urbiincludedir.
URBI_PACKAGE_KIND_SWITCH(
   [kernel], [AC_SUBST([urbiincludedir], ['${kernelincludedir}/urbi'])],
	     [AC_SUBST([urbiincludedir], ['${includedir}/urbi'])])

# /usr/local/gostai/core/$host
URBI_PACKAGE_KIND_SWITCH(
  [core\|sdk], [AC_SUBST([hostdir], ['${branddir}/core/${URBI_HOST}'])],
  [kernel],    [AC_SUBST([hostdir], ['${branddir}/core/${URBI_HOST}'])])

# /usr/local/gostai/core/$host/$env.
# Could have been named sdklibdir too.
AC_SUBST([envdir], ['${hostdir}/${URBI_ENV}'])
# Possibly an alias: /usr/local/gostai/kernel/powerpc-apple-darwin8.7.0/engine
URBI_PACKAGE_KIND_SWITCH([sdk], [],
	[AC_SUBST([kerneldir],
		  ['${kbranddir}/kernel/${URBI_HOST}/${URBI_ENV}'])])

# Where we install standard uobjects.
# /usr/local/gostai/core/$host/uobjects.
AC_SUBST([uobjectsdir], ['${hostdir}/uobjects'])


# Where we install, and expect to find, headers.
URBI_PACKAGE_KIND_SWITCH([core\|sdk],
	[AC_SUBST([sdkincludedir], ['${branddir}/core/include'])])
URBI_PACKAGE_KIND_SWITCH([sdk], [],
	[AC_SUBST([kernelincludedir], ['${kbranddir}/kernel/include'])])

# When compiling a core, we need to find the kernel headers and libraries.
URBI_PACKAGE_KIND_SWITCH(
  [core],
   [AC_SUBST([KERNEL_CPPFLAGS], ['-I${kernelincludedir}'])
    AC_SUBST([KERNEL_LDFLAGS],  ['-L${envdir}'])],
  [sdk],
   [AC_SUBST([SDK_CPPFLAGS], ['-I${sdkincludedir} -I${kernelincludedir}'])
    AC_SUBST([SDK_LDFLAGS],  ['-L${envdir}'])])


# Where we install umain.cc.  Used by umake.
AC_SUBST([umaindir], ['${branddir}/share/umain'])
])


## Local Variables:
## mode: autoconf
## End:
