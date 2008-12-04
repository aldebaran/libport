# See fix-libtool-la for more documentation.

%.la.stamp: %.la
	$(build_aux_dir)/fix-libtool-la $< $@

LTLIBRARIES_STAMPS = $(LTLIBRARIES:=.stamp)
CLEANFILES += $(LTLIBRARIES_STAMPS)
EXTRA_DIST += $(build_aux_dir)/fix-libtool-la
all-local: $(LTLIBRARIES_STAMPS)

