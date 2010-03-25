TESTS += $(LIBPORTIFY_TESTS)
LIBPORTIFY_TESTS = tests/bin/libportify/sample-1.lpc

# libportify-check
LPC = tests/bin/libportify-check
EXTRA_DIST += $(LPC).m4sh
nodist_check_SCRIPTS += $(LPC)
m4sh_scripts += $(LPC)

%.log: %.lpc %.exp $(LPC)
	@$(am__check_pre) $(LPC) $${dir}$< $(am__check_post)

