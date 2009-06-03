lib_LTLIBRARIES += lib/serialize/libserialize.la
lib_serialize_libserialize_la_LDFLAGS = -avoid-version -no-undefined
lib_serialize_libserialize_la_LIBADD = $(LIBADD) $(TINYXML_LIBS)
lib_serialize_libserialize_la_CPPFLAGS =	\
  $(AM_CPPFLAGS)				\
  -DBUILDING_SERIALIZE

dist_lib_serialize_libserialize_la_SOURCES =	\
  lib/serialize/binary-i-serializer.cc		\
  lib/serialize/binary-o-serializer.cc		\
  lib/serialize/exception.cc
