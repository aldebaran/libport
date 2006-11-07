# Before including this makefile snippet, define:

$(PREFIXED_CONFIG_HEADER): $(CONFIG_HEADER)
	@if test ! -f $@; then \
	  rm -f $(PREFIXED_CONFIG_HEADER).stamp; \
	  $(MAKE) $(AM_MAKEFLAGS) $(PREFIXED_CONFIG_HEADER).stamp; \
	else :; fi

$(PREFIX_CONFIG_H).stamp: $(CONFIG_HEADER) $(top_builddir)/config.status
	@rm -f $@
	cd $(top_builddir) && $(SHELL) ./config.status $(PREFIXED_CONFIG_HEADER)
	@touch $@

CLEANFILES +=					\
_configs.sed					\
$(PREFIXED_CONFIG_HEADER) 			\
$(PREFIXED_CONFIG_HEADER).stamp
