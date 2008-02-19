#
# $Id$
# Copyright (c) Gostai S.A.S., 2006.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com

include build-aux/build-aux.mk
include build-aux/config-headers-prefixed.mk

ACLOCAL_AMFLAGS = -I build-aux

SUBDIRS = lib/libport doc tests
# SUBDIRS += gui
EXTRA_DIST += bootstrap

include_libport = include/libport
include include/libport/libport.mk

.PHONY: doc html
doc html:
	cd doc && $(MAKE) $(AM_MAKEFLAGS) $@

check-buildfarm: check

# Manually list the files you're using from build-aux here.
# Note: Automake already takes care of the .mk and .m4 files you use.
EXTRA_DIST += \
		build-aux/test.sh
