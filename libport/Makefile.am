# This file is only activated by files that want to *check* libport.
# Should be liburbi only to save cycles.
# 
# Note that since several packages install it, they overwrite each
# others' libport/config.h (because of PACKAGE_STRING for instance),
# which triggers useless recompilations.

include $(top_srcdir)/build-aux/init.mk
include $(top_srcdir)/libport/libport.mk
SUBDIRS = tests

