## INSTRUMENT is the instrumentation tool.  INSTRUMENTATION is the
## probability (from 0 to 1) to activate instrumentation.

INSTRUMENT = $(build_aux_srcdir)/instrument
INSTRUMENTFLAGS = --libtool=$(abs_top_builddir)/libtool
RUN_INSTRUMENT = $(INSTRUMENT) $(INSTRUMENTFLAGS)
