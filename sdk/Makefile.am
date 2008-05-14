# libtool is required by umake.
host_SCRIPTS = $(top_builddir)/libtool

# This file tells umake how to build things.
nodist_env_DATA = param.mk

UMAKE_WRAPPERS = umake-engine umake-lib umake-aibo umake-remote umake-deepclean
UMAKE_CONFIGURED = umake uinstall umake-link
nodist_bin_SCRIPTS = $(UMAKE_WRAPPERS)
dist_bin_SCRIPTS = $(UMAKE_CONFIGURED)

$(UMAKE_WRAPPERS): wrapper.sh
	./wrapper.sh $@
$(UMAKE_CONFIGURED): umake-common

CLEANFILES = $(nodist_bin_SCRIPTS)

EXTRA_DIST = umake-common eval_makefile.pl

## ------------- ##
## param.mk.in.  ##
## ------------- ##

# There are macros that we want to override, we might not want to
# import all the macros from the Makefiles.  It would be simpler though.
#
#$(srcdir)/param.mk.in: $(srcdir)/param.mk.bottom $(srcdir)/configure.ac
#	autoconf -t 'AC_SUBST:$$1 = @$$1@' | sort -u  >$@.tmp
#	cat $(srcdir)/param.mk.bottom                >>$@.tmp
#	mv $@.tmp $@
