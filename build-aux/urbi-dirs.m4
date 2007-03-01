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
# kbranddir	   = ${URBI_KERNEL_PATH}/${PACKAGE_BRAND}
#                    /usr/local/gostai
# urbiincludedir   = ${includedir}/urbi
#                    /usr/local/include/urbi
# hostdir	   = ${branddir}/KIND/${URBI_HOST}
# envdir	   = ${hostdir}/${URBI_ENV}
#                    /usr/local/gostai/core/aibo
# kerneldir	   = ${kbranddir}/kernel/${URBI_HOST}/${URBI_ENV}
#                    /usr/local/gostai/kernel/aibo
# sdkincludedir	   = ${branddir}/KIND/include
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
  '':'') URBI_HOST=$host;;
  '':* ) URBI_HOST=$host_alias;;
   *:* ) URBI_HOST=$enable_host;;
esac
AC_MSG_RESULT([$URBI_HOST])
AC_SUBST([URBI_HOST])

# URBI_ENV
AC_ARG_ENABLE([env],
	      [AC_HELP_STRING([--enable-env=urbi-env],
			      [The environment this will run on:
			       aibo, webots, engine [$1]])])
AC_MSG_CHECKING([for URBI environment type])
case $enable_env in
 '') URBI_ENV=$1;;
  *) URBI_ENV=$enable_env;;
esac
AC_MSG_RESULT([$URBI_ENV])
case $URBI_ENV in
  aibo)
     AC_DEFINE([URBI_ENV_AIBO], [1], [Define if compiling for Aibo.]);;
  engine)
     AC_DEFINE([URBI_ENV_ENGINE], [1], [Define if compiling generic engine.]);;
  webots)
     AC_DEFINE([URBI_ENV_WEBOTS], [1], [Define if compiling for Webots.]);;
  *)
     AC_MSG_NOTICE([[unknown environment type: $URBI_ENV]]);;
esac
AC_SUBST([URBI_ENV])

# Everything is installed in $URBI_KERNEL_PATH/gostai.
AC_SUBST([PACKAGE_BRAND], [gostai])
AC_SUBST([branddir], ['${prefix}/${PACKAGE_BRAND}'])
URBI_PACKAGE_KIND_SWITCH(
  [core],   [AC_SUBST([kbranddir], ['${URBI_KERNEL_PATH}/${PACKAGE_BRAND}'])],
  [kernel], [AC_SUBST([kbranddir], ['${branddir}'])])

# Standard headers and libraries are installed in regular includedir
# and libdir under $prefix.  urbi-sdk headers are installed in
# urbiincludedir.
AC_SUBST([urbiincludedir], ['${includedir}/urbi'])

# /usr/local/gostai/core/$host
URBI_PACKAGE_KIND_SWITCH(
  [core\|sdk], [AC_SUBST([hostdir], ['${branddir}/core/${URBI_HOST}'])],
  [kernel],    [AC_SUBST([hostdir], ['${branddir}/kernel/${URBI_HOST}'])])

# /usr/local/gostai/core/$host/$env.
# Could have been named sdklibdir too.
AC_SUBST([envdir], ['${hostdir}/${URBI_ENV}'])
# Possibly an alias: /usr/local/gostai/kernel/powerpc-apple-darwin8.7.0/engine
URBI_PACKAGE_KIND_SWITCH([sdk], [],
	[AC_SUBST([kerneldir],
		  ['${kbranddir}/kernel/${URBI_HOST}/${URBI_ENV}'])])

# Where we install, and expect to find, headers.
URBI_PACKAGE_KIND_SWITCH([core\|sdk],
	[AC_SUBST([sdkincludedir], ['${branddir}/core/include'])])
URBI_PACKAGE_KIND_SWITCH([sdk], [],
	[AC_SUBST([kernelincludedir], ['${kbranddir}/kernel/include'])])

URBI_PACKAGE_KIND_SWITCH(
  [core\|sdk],
   [AC_SUBST([SDK_CPPFLAGS], ['-I${sdkincludedir} -I${kernelincludedir}'])
    AC_SUBST([SDK_LDFLAGS],  ['-L${envdir}'])])
])



# These are obsolete, and should be replaced by their current value.

# To be used by cores and engines.
AC_DEFUN([URBI_CORE_DIRS],
[URBI_PACKAGE_KIND([core])
URBI_DIRS([$1])])

# To be used by urbi-sdk and sdks.
AC_DEFUN([URBI_SDK_DIRS],
[URBI_PACKAGE_KIND([sdk])
URBI_DIRS([$1])])

# To be used by core developers for the kernels.
AC_DEFUN([URBI_KERNEL_DIRS],
[URBI_PACKAGE_KIND([kernel])
URBI_DIRS([$1])])

## Local Variables:
## mode: autoconf
## End:
