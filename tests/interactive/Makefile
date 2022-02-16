SUBDIRS=01_OK 02_TLE 03_FAIL
.PHONY=$(SUBDIRS) test force

test: $(SUBDIRS)

$(SUBDIRS): force
	@echo -e "\033[34mtesting interactive/$@\033[0m"
	@cd $@ && \
	./do.sh && \
	cd ..

force: ;