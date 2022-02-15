SUBDIRS=01_OK
.PHONY=$(SUBDIRS) test force

test: $(SUBDIRS)

$(SUBDIRS): force
	cd $@ && \
	./do.sh && \
	cd ..

force: ;