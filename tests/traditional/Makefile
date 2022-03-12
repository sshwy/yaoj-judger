SHELL=/bin/bash
SUBDIRS=01_DSC 02_OK 03_TLE 04_V_MLE 05_A_MLE 06_S_MLE 07_OLE 08_DSC 09_FILEIO_OK 10_DSC
.PHONY=$(SUBDIRS)

test: $(SUBDIRS)

$(SUBDIRS): force
	$(info testing traditional/$@)
	$(MAKE) test -C $@ PROJECT_ROOT=../$(PROJECT_ROOT)

force: ;

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir PROJECT_ROOT=../$(PROJECT_ROOT); \
	done 
