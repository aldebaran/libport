##
## qtestlib.mk: This file is part of build-aux.
## Copyright (C) Gostai S.A.S., 2007.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# DOCUMENTATION
# ---
# Enable filtering of QTestLib programs in order to be compatible with Emacs.
# If you include this Makefile snippets in one of your Makefile you must add
# ENABLE_EMACS_OUTPUT in your configure.ac file.

if COND_EMACS_OUTPUT
  TESTS_ENVIRONMENT = $(top_srcdir)/build-aux/emacsen-qtestlib.rb
endif
