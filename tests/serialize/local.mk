## Copyright (C) 2009-2011, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

TESTS_BINARIES +=				\
  tests/serialize/serialize.cc

tests_serialize_serialize_SOURCES = tests/serialize/serialize.cc
tests_serialize_serialize_LDFLAGS = $(SERIALIZE_LIBS) $(AM_LDFLAGS)

CHECK_CLEANFILES +=				\
  tests/serialize/binary_class			\
  tests/serialize/binary_hier			\
  tests/serialize/binary_integers_size		\
  tests/serialize/binary_pod			\
  tests/serialize/test.bin			\
  tests/serialize/test.xml
