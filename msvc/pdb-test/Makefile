# -Zi is used until -ggdb is translated to -Zi.
DEBUG=--runtime=debug  -ggdb -Zi

CXX=cl.exe

EXE=main_bin.exe # not main.exe to check *.pdb names.
CXXFLAGS=-wd4003 -wd4668
OBJS=main.obj
LIB_OBJS=bar.obj baz.obj
DLL=baz_lib.dll

all: run

# The Linux Kernel handle PE executable and run them under wine.
run: $(EXE)
	$< > $@

$(EXE): $(OBJS) $(DLL) Makefile
	$(CXX) $(CXXFLAGS) $(DEBUG) -o $@ $(OBJS) $(DLL)

$(DLL): $(LIB_OBJS)
	$(CXX) -o $@ $(LIB_OBJS) $(DEBUG) -link -dll

%.obj: %.cc Makefile
	$(CXX) $(CXXFLAGS) $(DEBUG) -c -o $@ $<

clean:
	-rm -f *.obj *.pdb *.ilk run $(EXE) $(OBJS) $(DLL:.dll=.exp) $(DLL:.dll=.lib) $(DLL)
