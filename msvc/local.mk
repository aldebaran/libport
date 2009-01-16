include $(top_srcdir)/build-aux/m4sh.mk

EXTRA_DIST +=					\
  msvc/cl.exe					\
  msvc/cppoutput				\
  msvc/cygpath					\
  msvc/lib					\
  msvc/lib.exe					\
  msvc/link.exe					\
  msvc/makedepend

# compilation
EXTRA_DIST += msvc/compilation.m4sh
nodist_check_SCRIPTS += msvc/compilation
m4sh_scripts += msvc/compilation

msvc/compilation.log: msvc/compilation
	@$(am__check_pre) $${dir}$< $(am__check_post)

MSVC_TESTS = msvc/compilation
TESTS += $(MSVC_TESTS)
