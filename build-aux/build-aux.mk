.PHONY: baux-up baux-ci baux-pin

# Update a pinned svn:externals.
baux-up:
	cd $(top_srcdir) && build-aux/svn-externals --update=build-aux

# Check in a pinned external from a host project, and depend upon it.
baux-ci:
	cd $(top_srcdir)/build-aux && vcs-svn ci
	$(MAKE) $(AM_MAKEFLAGS) baux-up

# Pin the svn:external by subscribing to it.
baux-pin:
	cd $(top_srcdir) && build-aux/svn-externals --subscribe=baux
