## Create the .version and .tarball-version files.
##
## This file is not meant to processed by Automake, which dislikes
## that we play tricks with VERSION and so forth.  So include it in a
## GNUmakefile.  Which also explains why we have to ship it
## "manually".
EXTRA_DIST +=					\
  build-aux/git-version-gen			\
  build-aux/move-if-change			\
  build-aux/package-version.mk

VERSION = $(shell cat $(VERSION_FILE))

## ----------------------------------------------------------- ##
## When rolling a tarball, update the current version string.  ##
## ----------------------------------------------------------- ##

# .version
#
# This file is always kept up to date.  Depend on it if you want
# accurate version information.  Note that it can be very costly to
# have C/C++ files depend on it, especially if they go into a library
# (which will require relinking etc.).
#
# Declare it "PHONY" to force a systematic update.
VERSION_FILE = $(top_srcdir)/.version
.PHONY: $(VERSION_FILE)
all: $(VERSION_FILE)
EXTRA_DIST += $(VERSION_FILE)
$(VERSION_FILE):
	$(build_aux_dir)/git-version-gen --srcdir=$(srcdir) --file --output=$@
