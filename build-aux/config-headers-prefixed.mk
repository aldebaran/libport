##
## config-headers-prefixed.mk: This file is part of build-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# Use this file only from the Makefile having the prefixed config.h.
BUILT_SOURCES += config.h

# There is no point in maintaining a stamp file, Automake already does
# this for config.h.  Arguably, we could use _configs.sed, but it is
# not documented.

config.h: $(CONFIG_HEADER)
	echo >&2 "*** This piece of Makefile should not be used."
	echo >&2 "*** If you want to, explain this to Akim."
	false
# Make sure we don't work with undefined vars.
	test -n "$(PREFIXED_CONFIG_HEADER)"
	cd $(top_builddir) &&					\
	$(SHELL) ./config.status $(PREFIXED_CONFIG_HEADER)

CLEANFILES +=					\
$(top_builddir)/_configs.sed			\
config.h
