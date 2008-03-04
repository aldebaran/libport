# This file can be included from anywhere, not just the top-level
# Makefile.am.  Define lib_libport as the path from the includer to
# here.

## --------- ##
## libport.  ##
## --------- ##

# Where we find libport sources in this package.
libport_srcdir   = $(srcdir)/$(lib_libport)
libport_builddir = $(builddir)/$(lib_libport)
# The library itself is compiled by the host directory.
# We might stop that some day and have libport be an autonomous dir.
libport_la = $(top_builddir)/lib/libport/libport.la

# Sources to compile to use libport.
libport_sources =				\
	$(libport_srcdir)/cli.cc		\
	$(libport_srcdir)/contract.cc		\
	$(libport_srcdir)/deref.cc		\
	$(libport_srcdir)/escape.cc		\
	$(libport_srcdir)/path.cc		\
	$(libport_srcdir)/file-library.cc	\
	$(libport_srcdir)/file-system.cc	\
	$(libport_srcdir)/indent.cc		\
	$(libport_srcdir)/package-info.cc	\
	$(libport_srcdir)/perror.cc		\
	$(libport_srcdir)/program-name.cc	\
	$(libport_srcdir)/read-stdin.cc		\
	$(libport_srcdir)/sched.cc		\
	$(libport_srcdir)/string.cc		\
	$(libport_srcdir)/symbol.cc		\
	$(libport_srcdir)/sysexits.cc		\
	$(libport_srcdir)/timer.cc		\
	$(libport_srcdir)/tokenizer.cc		\
	$(libport_srcdir)/ufloat.cc		\
	$(libport_srcdir)/utime.cc

# These are broken and someone will have to fix them...
# libport_sources += 				\
# 	$(libport_srcdir)/uffloat.cc		\
# 	$(libport_srcdir)/ull-fixed-point.cc
