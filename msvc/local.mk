## Copyright (C) 2009-2011, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

EXTRA_DIST +=					\
  msvc/cl.exe					\
  msvc/cygpath					\
  msvc/dumpbin.exe				\
  msvc/filter-cl-logs				\
  msvc/lib					\
  msvc/lib.exe					\
  msvc/link.exe					\
  msvc/makedepend				\
  msvc/msvc-common.sh

# compilation
EXTRA_DIST += msvc/compilation.m4sh
nodist_check_SCRIPTS += msvc/compilation
m4sh_scripts += msvc/compilation

msvc/compilation.log: msvc/compilation
	@$(am__check_pre) $${dir}$< $(am__check_post)

# depcomp
EXTRA_DIST += msvc/depcomp.m4sh
nodist_check_SCRIPTS += msvc/depcomp
m4sh_scripts += msvc/depcomp

msvc/depcomp.log: msvc/depcomp
	@$(am__check_pre) $${dir}$< $(am__check_post)

MSVC_TESTS = msvc/compilation msvc/depcomp
TESTS += $(MSVC_TESTS)
