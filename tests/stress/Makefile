SUBDIRS=02_RE 03_RTLE 04_CTLE
.PHONY=$(SUBDIRS) test force

test: $(SUBDIRS)

$(SUBDIRS): force
	cd $@ && \
	./do.sh && \
	cd ..

force: ;