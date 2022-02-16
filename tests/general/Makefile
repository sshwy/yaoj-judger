SUBDIRS=01_OK
.PHONY=$(SUBDIRS) test force clean

test: $(SUBDIRS)

$(SUBDIRS): force
	@echo -e "\033[34mtesting general/$@\033[0m"
	@cd $@ && \
	./do.sh && \
	cd ..

clean:
	@for dir in $(SUBDIRS); do \
		cd $$dir && \
		$(RM) main.local .log.local *.err *.out && \
		cd ..; \
	done

force: ;