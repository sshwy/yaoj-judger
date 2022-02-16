SUBDIRS=02_RE 03_RTLE 04_CTLE 05_SEGV 06_USR1
.PHONY=$(SUBDIRS) test force

test: $(SUBDIRS)

$(SUBDIRS): force
	@echo -e "\033[34mtesting stress/$@\033[0m"
	@cd $@ && \
	./do.sh && \
	cd ..

force: ;