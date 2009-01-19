lib_LTLIBRARIES += lib/tinyxml/libtinyxml.la
dist_lib_tinyxml_libtinyxml_la_SOURCES = $(lib_tinyxml_libtinyxml_sources)
lib_tinyxml_libtinyxml_la_LDFLAGS =
lib_tinyxml_libtinyxml_la_CPPFLAGS =		\
  $(AM_CPPFLAGS)				\
  -DTIXML_USE_STL				\
  -I$(srcdir)/include/tinyxml

dist_lib_tinyxml_libtinyxml_la_SOURCES +=	\
  lib/tinyxml/tinystr.cpp			\
  lib/tinyxml/tinyxml.cpp			\
  lib/tinyxml/tinyxmlerror.cpp			\
  lib/tinyxml/tinyxmlparser.cpp

# This is distributed, but not compiled directly: it is included by
# Coro.cc.
EXTRA_DIST += lib/tinyxml/libcoroutine/Coro.c
