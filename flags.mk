include $(top_srcdir)/build-aux/init.mk
AUTOMAKE_OPTIONS += nostdinc
AM_CPPFLAGS += $(LIBPORT_CPPFLAGS)
AM_CXXFLAGS += $(WARNING_CXXFLAGS)
