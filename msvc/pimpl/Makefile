CXX = cl.exe
all:
	$(CXX) -DBUILD auto_pimpl.cc -c -o auto_pimpl.obj
	$(CXX) -DBUILD client.cc -c -o client.obj
	$(CXX) auto_pimpl.obj client.obj -o client

EXTRA_DIST = Makefile auto_pimpl.cc auto_pimpl.h client.cc

distdir = pimpl
dist:
	rm -rf $(distdir)
	mkdir $(distdir)
	cp $(EXTRA_DIST) $(distdir)
	tar zcf $(distdir).tgz $(distdir)
	rm -rf $(distdir)

check: all
	./client
