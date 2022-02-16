SUBDIRS=02_RE 03_RTLE 04_CTLE 05_SEGV 06_USR1 07_POLICY
.PHONY=$(SUBDIRS) test force clean

test: $(SUBDIRS)

$(SUBDIRS): force
	@echo -e "\033[34mtesting stress/$@\033[0m"
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