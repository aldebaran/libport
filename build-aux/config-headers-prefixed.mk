# Use this file only from the Makefile having the prefixed config.h.
BUILT_SOURCES += config.h

# There is no point in maintaining a stamp file, Automake already does
# this for config.h.  Arguably, we could use _configs.sed, but it is
# not documented.

config.h: $(CONFIG_HEADER)
	cd $(top_builddir) &&					\
	$(SHELL) ./config.status $(PREFIXED_CONFIG_HEADER)

CLEANFILES +=					\
_configs.sed					\
config.h
