LIBPORTIFY_TESTS = $(call ls_files,tests/bin/libportify/*.lpc)
TESTS += $(LIBPORTIFY_TESTS)
EXTRA_DIST += $(LIBPORTIFY_TESTS) $(LIBPORTIFY_TESTS:.lpc=.exp)

# libportify-check
LPC = tests/bin/libportify-check
EXTRA_DIST += $(LPC).m4sh
nodist_check_SCRIPTS += $(LPC)
m4sh_scripts += $(LPC)

%.log: %.lpc %.exp $(LPC) bin/libportify
	@$(am__check_pre) $(LPC) $${dir}$< $(am__check_post)

