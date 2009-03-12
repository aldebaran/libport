# See fix-libtool-la for more documentation.  Basically, just include
# this file where Libtool libraries are defined.

%.la.stamp: %.la
	$(build_aux_dir)/fix-libtool-la $< $@

LTLIBRARIES_STAMPS = $(LTLIBRARIES:=.stamp)
CLEANFILES += $(LTLIBRARIES_STAMPS) $(LTLIBRARIES:=.bak)
EXTRA_DIST += $(build_aux_dir)/fix-libtool-la
all-local: $(LTLIBRARIES_STAMPS)



## ------------------------------------------------------ ##
## Workaround installed libtool libraries under windows.  ##
## ------------------------------------------------------ ##

# Our trick, defining libdir = ${bindir} so that under Windows the
# dlls are found where expected, does not work perfectly.  In
# particular libtool might issue an *.la containing:
#
# # The name that we can dlopen(3).
# dlname='../bin/libuobject.dll'
#
# and dlopen fails.  Note that "../bin" is useless, we just got out of
# bin, and enter again.  If we remove this indirection, everything
# works.  If we remove the la file, ltdl finds the *.dll file and
# again, it works.  It has been reported several times that installing
# *.la files makes no sense.  Yet, I (Akim), am a bit worried of
# getting rid of the *.la files, they can be useful later.  Besides,
# if we remove them, libtool is unable to uninstall the libraries,
# which results in distcheck failures.
#
# Now the question is "where are the libtool libraries we just
# installed?"  As already used above, LTLIBRARIES includes all the
# (builddir) *.la files.  They feature a line:
#
# # Directory that this library needs to be installed in:
# libdir='/usr/local/bin'
#
# which is what we need.  Don't forget DESTDIR though.
#
# libdir is empty for convenience libraries, which we don't have
# to process.
install-exec-hook:
	-case "$(URBI_HOST):$(LTLIBRARIES)" in		\
	  (*:) ;;					\
	  (*mingw*:*)					\
	    for lib in $(LTLIBRARIES); do		\
	      libdir=$$(source $$lib && echo $$libdir);	\
	      test -n "$$libdir" || continue;		\
	      perl -pi -e "s{^dlname='.*/}{dlname='}g"	\
	        $(DESTDIR)$$libdir/$$(basename $$lib);	\
	    done;;					\
	esac
