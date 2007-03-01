# This file can be included from anywhere, not just the top-level
# Makefile.am.

## --------- ##
## libport.  ##
## --------- ##

# Where we find libport sources in this package.
libport_srcdir = $(top_srcdir)/libport
libport_builddir = $(top_builddir)/libport

# URBI_LIBPORT defines where we install libport.
libportsysdir = $(libportdir)/sys

# Sources to compile to use libport.
libport_sources = 				\
	$(libport_srcdir)/escape.cc

# These are broken and someone will have to fix them...
# libport_sources += 				\
# 	$(libport_srcdir)/ufloat.cc		\
# 	$(libport_srcdir)/uffloat.cc		\
# 	$(libport_srcdir)/ull-fixed-point.cc
