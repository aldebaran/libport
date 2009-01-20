lib_LTLIBRARIES += lib/serialize/libserialize.la
lib_serialize_libserialize_la_LDFLAGS = -avoid-version -no-undefined
lib_serialize_libserialize_la_LIBADD = $(LIBADD) lib/tinyxml/libtinyxml.la
lib_serialize_libserialize_la_CPPFLAGS =	\
  $(AM_CPPFLAGS)				\
  -I$(srcdir)/lib				\
  -DBUILDING_SERIALIZE				\
  $(BOOST_CPPFLAGS)

dist_lib_serialize_libserialize_la_SOURCES =	\
  lib/serialize/exception.cc			\
  lib/serialize/i-serializer.cc			\
  lib/serialize/o-serializer.cc			\
  lib/serialize/serializer.cc			\
  lib/serialize/xml-i-serializer.cc		\
  lib/serialize/xml-o-serializer.cc
