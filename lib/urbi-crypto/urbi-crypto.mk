##
## urbi-crypto.mk: This file is part of urbi-crypto.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

## ------------- ##
## urbi-crypto.  ##
## ------------- ##

include $(top_srcdir)/build-aux/svn-externals.mk

noinst_LTLIBRARIES = liburbicrypto.la

# Our header and source dirs.
urbi_crypto_headers_dir = $(top_srcdir)/include/urbi-crypto
urbi_crypto_sources_dir = $(top_srcdir)/lib/urbi-crypto

urbi_crypto_includedir = $(libport_include_basedir)/libport/urbi-crypto

dist_urbi_crypto_include_HEADERS =			\
	$(urbi_crypto_headers_dir)/exception.hh		\
	$(urbi_crypto_headers_dir)/debug-log.hh		\
	$(urbi_crypto_headers_dir)/utility.hh		\
	$(urbi_crypto_headers_dir)/xutility.hh		\
	$(urbi_crypto_headers_dir)/openssl.hh		\
	$(urbi_crypto_headers_dir)/sign.hh		\
	$(urbi_crypto_headers_dir)/signsha1.hh		\
	$(urbi_crypto_headers_dir)/crypto.hh		\
	$(urbi_crypto_headers_dir)/crypt.hh		\
	$(urbi_crypto_headers_dir)/mangle-defines.hh	\
	$(urbi_crypto_headers_dir)/cryptrsa.hh		\
	$(urbi_crypto_headers_dir)/cryptaes.hh

liburbicrypto_la_SOURCES =			\
	$(urbi_crypto_sources_dir)/debug-log.cc	\
	$(urbi_crypto_sources_dir)/utility.cc	\
	$(urbi_crypto_sources_dir)/openssl.cc	\
	$(urbi_crypto_sources_dir)/sign.cc	\
	$(urbi_crypto_sources_dir)/signsha1.cc	\
	$(urbi_crypto_sources_dir)/crypto.cc	\
	$(urbi_crypto_sources_dir)/crypt.cc	\
	$(urbi_crypto_sources_dir)/cryptrsa.cc	\
	$(urbi_crypto_sources_dir)/cryptaes.cc

liburbicrypto_la_CPPFLAGS = $(AM_CPPFLAGS) $(OPENSSL_CPPFLAGS)
liburbicrypto_la_LIBADD = $(OPENSSL_LDFLAGS)

EXTRA_DIST += $(urbi_crypto_sources_dir)/applink.c


## ------------ ##
## svn sugars.  ##
## ------------ ##

# svn-externals.mk must be included.
SVN_EXTERNALS += urbi-crypto

