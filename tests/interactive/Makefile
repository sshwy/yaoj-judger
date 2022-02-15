SUBDIRS=01_OK 02_TLE 03_FAIL
.PHONY=$(SUBDIRS) test force

test: $(SUBDIRS)

$(SUBDIRS): force
	cd $@ && \
	./do.sh && \
	cd ..

force: ;