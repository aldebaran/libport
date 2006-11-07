# This file is only activated by files that want to *check and
# install* libport.  Should be liburbi only.
# 
# Note that if several packages install it, as a result they overwrite
# each others' libport/config.h (because of PACKAGE_STRING for
# instance), which triggers useless recompilations.

include $(top_srcdir)/build-aux/init.mk
include $(top_srcdir)/libport/libport.mk
libport_HEADERS = $(libport_headers)
nodist_libport_HEADERS = $(nodist_libport_headers)

SUBDIRS = tests

