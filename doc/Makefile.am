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

DOXYGEN = doxygen

.PHONY: doc html

doc: html

html-local: Doxyfile
	$(DOXYGEN)

Doxyfile: doxygen.sh
	./doxygen.sh

clean-local:
	rm -rf html latex

EXTRA_DIST = doxygen.sh.in
CLEANFILES = Doxyfile doxygen.sh
