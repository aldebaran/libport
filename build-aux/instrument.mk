## Warning: we have a conflict between INSTRUMENT being the name of
## the instrumentation tool, and INSTRUMENT being a (fuzzy) Boolean
## stating where we should be instrumenting.

INSTRUMENT = $(build_aux_srcdir)/instrument
INSTRUMENTFLAGS =                                                      \
  --valgrind-option=--suppressions=$(build_aux_srcdir)/instrument.supp \
  --valgrind-option=--gen-suppressions=all                             \
  --libtool=$(abs_top_builddir)/libtool
RUN_INSTRUMENT = $(INSTRUMENT) $(INSTRUMENTFLAGS)
