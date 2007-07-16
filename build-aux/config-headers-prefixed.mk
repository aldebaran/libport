# Use this file only from the top-level Makefile, otherwise
# it won't produce the expect result.

BUILT_SOURCES += $(PREFIXED_CONFIG_HEADER)

# There is no point in maintaining a stamp file, Automake already does
# this for config.h.  Arguably, we could use _configs.sed, but it is
# not documented.

$(PREFIXED_CONFIG_HEADER): $(CONFIG_HEADER)
	cd $(top_builddir) &&					\
	$(SHELL) ./config.status $(PREFIXED_CONFIG_HEADER)

CLEANFILES +=					\
_configs.sed					\
$(PREFIXED_CONFIG_HEADER)
