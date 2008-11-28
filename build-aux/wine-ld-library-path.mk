## Wine is not providing easy means to pass LD_LIBRARY_PATH, which is a
## problem especially for not-yet-installed libraries and executables.
##
## The script wine-ld-library-path implements the needed hack.  It
## should be called with its arguments given as usual on the command
## line, or in the envvar WINE_LD_LIBRARY_PATH_ARGS.  The point of this
## envvar is to allow not to use the wrapper at all if it is not
## needed.

wine_ld_library_path = $(build_aux_builddir)/wine-ld-library-path
EXTRA_DIST += $(build_aux_dir)/wine-ld-library-path.as
nodist_check_SCRIPTS += $(wine_ld_library_path)
m4sh_scripts += $(wine_ld_library_path)

RUN_WINE_LD_LIBRARY_PATH :=					\
  $(shell							\
     case $(host) in						\
       (*pw32*|*mingw32*) echo '$(wine_ld_library_path)';;	\
     esac)

clean-local: clean-wine-ld-library-path

.PHONY: clean-wine-ld-library-path
clean-wine-ld-library-path:
	-rm -rf wine

debug: debug-wine-ld-library-path
.PHONY: debug-wine-ld-library-path
debug-wine-ld-library-path:
	@echo 'RUN_WINE_LD_LIBRARY_PATH = $(RUN_WINE_LD_LIBRARY_PATH)'
	@echo 'TESTS_ENVIRONMENT = $(TESTS_ENVIRONMENT)'
