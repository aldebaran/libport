## ----------- ##
## Bootstrap.  ##
## ----------- ##

# boostrap itself touches the timestamp.
BUILT_SOURCES += $(srcdir)/bootstrap.stamp
MAINTAINERCLEANFILES += $(srcdir)/bootstrap.stamp

BOOTSTRAP_sources =				\
  $(BOOTSTRAP_CFG)				\
  $(build_aux_dir)/bootstrap			\
  $(srcdir)/bootstrap.stamp

dist_noinst_DATA += $(BOOTSTRAP_sources)

$(srcdir)/bootstrap.stamp: $(BOOTSTRAP_sources)
	@echo "Bootstrapping again because of $?"
	cd $(top_srcdir) && ./bootstrap
