# This file can be included from anywhere, not just the top-level
# Makefile.am.

## --------- ##
## libport.  ##
## --------- ##

# Where we find libport sources in this package.
libport_srcdir = $(top_srcdir)/libport
libport_builddir = $(top_builddir)/libport
# The library itself is compiled by the host directory.
# We might stop that some day and have libport be an autonomous dir.
libport_la = $(top_builddir)/libport/libport.la

# URBI_LIBPORT defines where we install libport.
libportsysdir = $(libportdir)/sys

# Sources to compile to use libport.
libport_sources = 				\
	$(libport_srcdir)/cli.cc		\
	$(libport_srcdir)/contract.cc		\
	$(libport_srcdir)/deref.cc		\
	$(libport_srcdir)/escape.cc		\
	$(libport_srcdir)/file-system.cc	\
	$(libport_srcdir)/indent.cc		\
	$(libport_srcdir)/package-info.cc	\
	$(libport_srcdir)/program-name.cc	\
	$(libport_srcdir)/symbol.cc		\
	$(libport_srcdir)/sysexits.cc		\
	$(libport_srcdir)/tokenizer.cc          \
	$(libport_srcdir)/utime.cc

# These are broken and someone will have to fix them...
# libport_sources += 				\
# 	$(libport_srcdir)/ufloat.cc		\
# 	$(libport_srcdir)/uffloat.cc		\
# 	$(libport_srcdir)/ull-fixed-point.cc
