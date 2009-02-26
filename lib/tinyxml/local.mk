lib_LTLIBRARIES += lib/tinyxml/libtinyxml.la
lib_tinyxml_libtinyxml_la_LDFLAGS = -avoid-version -no-undefined
lib_tinyxml_libtinyxml_la_CPPFLAGS =		\
  $(AM_CPPFLAGS)				\
  -DBUILDING_TINYXML				\
  -I$(srcdir)/include/tinyxml

dist_lib_tinyxml_libtinyxml_la_SOURCES =	\
  lib/tinyxml/tinystr.cpp			\
  lib/tinyxml/tinyxml.cpp			\
  lib/tinyxml/tinyxmlerror.cpp			\
  lib/tinyxml/tinyxmlparser.cpp
