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

ACLOCAL_AMFLAGS = -I build-aux

SUBDIRS = src gui doc tests
EXTRA_DIST = bootstrap

.PHONY: doc html
doc html:
	cd doc && $(MAKE) $(AM_MAKEFLAGS) $@
