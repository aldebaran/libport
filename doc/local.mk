## Copyright (C) 2006, 2009-2011, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

#
# Copyright (C) 2006, 2009, 2010 Gostai S.A.S.
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

CLEANDIRS += doc/html doc/latex

EXTRA_DIST += doc/doxygen.sh.in
CLEANFILES += doc/Doxyfile doc/doxygen.sh


## ------- ##
## LaTeX.  ##
## ------- ##

# Document aux.
share_dir = $(srcdir)/doc/document-aux
share_bin_dir = $(share_dir)/bin
share_make_dir = $(share_dir)/make
include doc/document-aux/make/tex.mk
EXTRA_DIST +=					\
  $(call ls_files,@{doc/document-aux})

TEXI2DVIFLAGS += -I $(top_srcdir) -I doc -~

MAKEINDEXFLAGS = -s headings.ist
TEX_ENVIRONMENT =				\
  PATH=$$PATH:$(abspath $(share_bin_dir))	\
  MAKEINDEX="makeindex $(MAKEINDEXFLAGS)"	\
  TEXINDY="texindy $(TEXINDYFLAGS)"		\
  TEX4HT="tex4ht -cunihtf -utf8"

PDF = doc/libport.pdf
EXTRA_DATA += $(PDF)
CLEANFILES += $(PDF)

REVISION_FILE = revision.sty
CLEANFILES += $(REVISION_FILE)
$(REVISION_FILE): | $(VERSIONIFY_CACHE)
VERSIONIFY_CACHE_RUN += --latex=$(REVISION_FILE)

libport_sources =				\
  $(call ls_files,doc/*.cc)			\
  $(call ls_files,doc/*.tex)
EXTRA_DIST += $(libport_sources)
libport_deps =					\
  $(REVISION_FILE)				\
  $(libport_sources)
#   $(call ls_files,doc/*.sty doc/*.cls)
$(PDF): $(libport_deps)
