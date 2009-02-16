AM_CPPFLAGS +=					\
  $(BOOST_CPPFLAGS)				\
  $(LIBPORT_CPPFLAGS)

AM_LDFLAGS +=                                   \
  $(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS)

LDADD +=                                        \
  $(LIBPORT_LIBS)			        \
  $(BOOST_UNIT_TEST_FRAMEWORK_LIBS)

TESTS +=					\
  serialize/serialize.cc

serialize_serialize_SOURCES = serialize/serialize.cc
serialize_serialize_LDFLAGS = $(SERIALIZE_LIBS) $(AM_LDFLAGS)

CHECK_CLEANFILES +=				\
  serialize/test.bin				\
  serialize/test.xml
