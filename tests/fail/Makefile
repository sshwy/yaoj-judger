SUBDIRS=01 02 03
.PHONY=$(SUBDIRS) test force

test: $(SUBDIRS)

$(SUBDIRS): force
	cd $@ && \
	./do.sh && \
	cd ..

force: ;
