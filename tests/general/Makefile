SUBDIRS=01_OK
.PHONY=$(SUBDIRS) test force

test: $(SUBDIRS)

$(SUBDIRS): force
	@echo -e "\033[34mtesting general/$@\033[0m"
	@cd $@ && \
	./do.sh && \
	cd ..

force: ;