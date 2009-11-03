## Define the path to libport components.
##
## We use to define these from URBI_LIBPORT using AC_SUBST, but then
## in "libfoo_LIBADD = $(LIBPORT_LIBS)" Automake fails to see that the
## value of LIBPORT_LIBS is actually a library, so it does not add it
## to the list of dependencies of libfoo.  So instead of AC_SUBST,
## which is opaque to Automake, we use Automake variables.

## Avoid useless "./" in file names.  This is not only nicer, it is
## also needed for Make to match properly libraries being build, and
## libraries being used.  Without that, builds may fail, as the
## dependencies are incorrect.
##
## LIBPORT_BUILDPREFIX must be empty, or end with a /.

## $(top_srcdir) to find sources, $(top_builddir) to find libport/config.h.
LIBPORT_CPPFLAGS = -I$(LIBPORT_SRCDIR)/include -I$(LIBPORT_BUILDPREFIX)include -I$(LIBPORT_SRCDIR)/libltdl
LIBPORT_LIBS = $(LIBPORT_BUILDPREFIX)lib/libport/libport.la

LTDL_CPPFLAGS = -I$(LIBPORT_SRCDIR) -I$(LIBPORT_SRCDIR)/libltdl
## Note that libltdl is linked in libport, so you should not try to
## link against libltdl.
LTDL_LIBS = $(LIBPORT_BUILDPREFIX)libltdl/libltdlc.la

SCHED_CPPFLAGS = $(LIBPORT_CPPFLAGS)
SCHED_LIBS = $(LIBPORT_BUILDPREFIX)lib/sched/libsched.la

SERIALIZE_CPPFLAGS = $(LIBPORT_CPPFLAGS)
SERIALIZE_LIBS = $(LIBPORT_BUILDPREFIX)lib/serialize/libserialize.la

TINYXML_CPPFLAGS = $(LIBPORT_CPPFLAGS)
TINYXML_LIBS = $(LIBPORT_BUILDPREFIX)lib/tinyxml/libtinyxml.la
