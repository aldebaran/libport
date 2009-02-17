#
# Copyright (C) 2006, 2009 Gostai S.A.S.
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

html-local: doc/Doxyfile
	$(DOXYGEN)

doc/Doxyfile: doc/doxygen.sh
	$< doc

clean-local:
	rm -rf doc/html doc/latex

EXTRA_DIST += doc/doxygen.sh.in
CLEANFILES += doc/Doxyfile doc/doxygen.sh
