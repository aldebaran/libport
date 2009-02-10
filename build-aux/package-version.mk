## Create the .version file.
##
## This Makefile is not meant to processed by Automake, which dislikes
## the tricks we play with VERSION and so forth.  So include it in a
## GNUmakefile.  Which also explains why we have to ship it
## "manually".
EXTRA_DIST +=					\
  build-aux/git-version-gen			\
  build-aux/move-if-change			\
  build-aux/package-version.mk

VERSION = $(shell sed -n '/^TarballVersion: /{s///;p;q;}' $(VERSION_FILE))

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

# This is not an Automake file, so don't use BUILT_SOURCES, "all" will
# do.
all: $(VERSION_FILE)

# Ship it, so that when we are detached from a repository, we still
# have it available.
EXTRA_DIST += $(VERSION_FILE)
$(VERSION_FILE):
	$(build_aux_dir)/git-version-gen --srcdir=$(top_srcdir) --cache=$@

debug: debug-revision
debug-revision:
	@echo "$$\(VERSION) = $(VERSION)"
