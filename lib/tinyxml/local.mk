lib_LTLIBRARIES += lib/tinyxml/libtinyxml@LIBSFX@.la
lib_tinyxml_libtinyxml@LIBSFX@_la_CPPFLAGS =	\
  $(AM_CPPFLAGS)				\
  -DBUILDING_TINYXML				\
  -I$(srcdir)/include/tinyxml

dist_lib_tinyxml_libtinyxml@LIBSFX@_la_SOURCES =	\
  lib/tinyxml/tinystr.cpp				\
  lib/tinyxml/tinyxml.cpp				\
  lib/tinyxml/tinyxmlerror.cpp				\
  lib/tinyxml/tinyxmlparser.cpp

lib_tinyxml_libtinyxml@LIBSFX@_la_LDFLAGS = -avoid-version -no-undefined
# We use libport.cassert.
lib_tinyxml_libtinyxml@LIBSFX@_la_LIBADD = $(LIBPORT_LIBS)
