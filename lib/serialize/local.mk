## Copyright (C) 2009-2010, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

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
