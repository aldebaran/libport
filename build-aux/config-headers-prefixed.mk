# There is no point in maintaining a stamp file, Automake already does
# this for config.h.  Arguably, we could use _configs.sed, but it is
# not documented.
BUILT_SOURCES += $(PREFIXED_CONFIG_HEADER)
$(PREFIXED_CONFIG_HEADER): $(CONFIG_HEADER)
	cd $(top_builddir) && \
	$(SHELL) ./config.status $(PREFIXED_CONFIG_HEADER)

CLEANFILES +=					\
_configs.sed					\
$(PREFIXED_CONFIG_HEADER)
