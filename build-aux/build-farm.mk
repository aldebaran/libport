# This is a modified version of Automake 1.10.2's distcheck.  The
# differences are:
#
# - we don't run "make check", as the build-farm already does before
#   running distcheck;
#
# - we introduce DISTCHECK_INSTALLCHECK_FLAGS which defaults to passing
#   VERBOSE to 1.
.PHONY: distcheck-buildfarm
distcheck-buildfarm: dist
	case '$(DIST_ARCHIVES)' in \
	*.tar.gz*) \
	  GZIP=$(GZIP_ENV) gunzip -c $(distdir).tar.gz | $(am__untar) ;;\
	*.tar.bz2*) \
	  bunzip2 -c $(distdir).tar.bz2 | $(am__untar) ;;\
	*.tar.lzma*) \
	  unlzma -c $(distdir).tar.lzma | $(am__untar) ;;\
	*.tar.Z*) \
	  uncompress -c $(distdir).tar.Z | $(am__untar) ;;\
	*.shar.gz*) \
	  GZIP=$(GZIP_ENV) gunzip -c $(distdir).shar.gz | unshar ;;\
	*.zip*) \
	  unzip $(distdir).zip ;;\
	esac
	chmod -R a-w $(distdir); chmod a+w $(distdir)
	mkdir $(distdir)/_build
	mkdir $(distdir)/_inst
	chmod a-w $(distdir)
	dc_install_base=`$(am__cd) $(distdir)/_inst && pwd | sed -e 's,^[^:\\/]:[\\/],/,'` \
	  && dc_destdir="$${TMPDIR-/tmp}/am-dc-$$$$/" \
	  && cd $(distdir)/_build \
	  && ../configure --srcdir=.. --prefix="$$dc_install_base" \
	    $(DISTCHECK_CONFIGURE_FLAGS) \
	  && $(MAKE) $(AM_MAKEFLAGS) \
	  && $(MAKE) $(AM_MAKEFLAGS) dvi \
##	  && $(MAKE) $(AM_MAKEFLAGS) check
	  && $(MAKE) $(AM_MAKEFLAGS) install \
	  && $(MAKE) $(AM_MAKEFLAGS) installcheck $(DISTCHECK_INSTALLCHECK_FLAGS) \
	  && $(MAKE) $(AM_MAKEFLAGS) uninstall \
	  && $(MAKE) $(AM_MAKEFLAGS) distuninstallcheck_dir="$$dc_install_base" \
	        distuninstallcheck \
	  && chmod -R a-w "$$dc_install_base" \
	  && ({ \
	       (cd ../.. && umask 077 && mkdir "$$dc_destdir") \
	       && $(MAKE) $(AM_MAKEFLAGS) DESTDIR="$$dc_destdir" install \
	       && $(MAKE) $(AM_MAKEFLAGS) DESTDIR="$$dc_destdir" uninstall \
	       && $(MAKE) $(AM_MAKEFLAGS) DESTDIR="$$dc_destdir" \
	            distuninstallcheck_dir="$$dc_destdir" distuninstallcheck; \
	      } || { rm -rf "$$dc_destdir"; exit 1; }) \
	  && rm -rf "$$dc_destdir" \
	  && $(MAKE) $(AM_MAKEFLAGS) dist \
	  && rm -rf $(DIST_ARCHIVES) \
	  && $(MAKE) $(AM_MAKEFLAGS) distcleancheck
	$(am__remove_distdir)
	@(echo "$(distdir) archives ready for distribution: "; \
	  list='$(DIST_ARCHIVES)'; for i in $$list; do echo $$i; done) | \
	  sed -e 1h -e 1s/./=/g -e 1p -e 1x -e '$$p' -e '$$x'

# The buildfarm should not try to render colors in the output, it
# should instrument the tests (that's the best moment to do that since
# we no longer run wrappers, but really the programs themselves), and
# it should report the logs, as otherwise it just display a big fat
# silent failure.
DISTCHECK_INSTALLCHECK_FLAGS ?=			\
  AM_COLOR_TESTS=no				\
  INSTRUMENT=1					\
  VERBOSE=1

# Remove the dists and distdirs that we made.
#
# Don't remove the current one, $(distdir).
#
# Don't use xargs when there might not be arguments, on GNU/Linux it
# runs the command anyway, without arguments, which results in useless
# noise on stderr.
.PHONY: dists-clean
dists-clean:
	for i in $(PACKAGE)-*;					\
	do							\
	  case $$i$$(test ! -d $$i || echo /) in		\
	    ($(distdir)*);;					\
	    (*.tar.gz | *.zip) remove+=" $$i";;			\
	    (*/) test ! -f $$i/configure || remove+=" $$i";;	\
	  esac;							\
	done;							\
	echo "Removing former distributions...";		\
	for i in $$remove;					\
	do							\
	  echo "... $$i";					\
	  find $$i -type d ! -perm -200 -exec chmod u+w {} ';';	\
	  rm -rf $$i;						\
	done;							\
	echo "... done"

# Always clean beforehand, because since our VERSION changes
# frequently, we leave bazillions of distdirs and dists.
distcheck-buildfarm: dists-clean
