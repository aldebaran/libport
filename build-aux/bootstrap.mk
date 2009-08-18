## ----------- ##
## Bootstrap.  ##
## ----------- ##

# boostrap itself touches the timestamp.
BUILT_SOURCES += $(srcdir)/bootstrap.stamp
MAINTAINERCLEANFILES += $(srcdir)/bootstrap.stamp

BOOTSTRAP_sources =				\
  $(BOOTSTRAP_CFG)				\
  $(build_aux_dir)/bootstrap

dist_noinst_DATA +=				\
  $(BOOTSTRAP_sources)				\
  $(srcdir)/bootstrap.stamp

$(srcdir)/bootstrap.stamp: $(BOOTSTRAP_sources) $(build_aux_dir)/bootstrap.mk
	@echo "Bootstrapping again because of $?"
	cd $(top_srcdir) && ./bootstrap
