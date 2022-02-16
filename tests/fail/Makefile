SUBDIRS=01 02 03
.PHONY=$(SUBDIRS) test force

test: $(SUBDIRS)

$(SUBDIRS): force
	@echo -e "\033[34mtesting fail/$@\033[0m"
	@cd $@ && \
	./do.sh && \
	cd ..

force: ;
