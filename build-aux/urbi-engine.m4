# This file tries to factor the common bits for URBI engines:
# initializing Autotools, defining standard directory variables
# (URBI_DIRS), generating the documentation, etc.  Some parts cannot
# be factored because of limitation in the Autotools, otherwise this
# would be named URBI_INIT for instance, and also include AC_INIT.
#
# Try to use it in every URBI engine, and keep their copy in sync.

m4_pattern_forbid([^ACX_])dnl
m4_pattern_forbid([^URBI_])dnl

AC_PREREQ([2.60])

# URBI_ENGINE([DEFAULT-URBI-ENV])
# -------------------------------
AC_DEFUN([URBI_ENGINE],
[# Automake.
AM_INIT_AUTOMAKE([foreign 1.9.6 -Wall dist-zip])


## ------------ ##
## Parameters.  ##
## ------------ ##

# Urbi standard locations.
URBI_DIRS([$1])

# doc/srcdoc.
URBI_DOC

# C++ Compiler.
URBI_PROG_CXX

# We don't need static libraries, speed the compilation up.
AC_ENABLE_SHARED([no])
AC_PROG_LIBTOOL

# Every engine comes with an SDK.
URBI_SDK


## -------- ##
## Output.  ##
## -------- ##

AC_CONFIG_FILES([Makefile])
])

## Local Variables:
## mode: autoconf
## End:
