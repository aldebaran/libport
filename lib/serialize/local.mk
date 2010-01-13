lib_LTLIBRARIES += lib/serialize/libserialize@LIBSFX@.la
lib_serialize_libserialize@LIBSFX@_la_LDFLAGS = -avoid-version -no-undefined
lib_serialize_libserialize@LIBSFX@_la_LIBADD = $(LIBADD) $(LIBPORT_LIBS)
lib_serialize_libserialize@LIBSFX@_la_CPPFLAGS =	\
  $(AM_CPPFLAGS)				\
  -DBUILDING_SERIALIZE

dist_lib_serialize_libserialize@LIBSFX@_la_SOURCES =	\
  lib/serialize/binary-i-serializer.cc		\
  lib/serialize/binary-o-serializer.cc		\
  lib/serialize/exception.cc
