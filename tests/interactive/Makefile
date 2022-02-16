SUBDIRS=01_OK 02_TLE 03_FAIL
.PHONY=$(SUBDIRS) test force clean

test: $(SUBDIRS)

$(SUBDIRS): force
	@echo -e "\033[34mtesting interactive/$@\033[0m"
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