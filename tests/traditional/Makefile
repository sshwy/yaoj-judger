SUBDIRS=01_DSC 02_OK 03_TLE 04_V_MLE 05_A_MLE 06_S_MLE 07_OLE 08_DSC 09_FILEIO_OK 10_DSC
.PHONY=$(SUBDIRS)

test:
	for dir in $(SUBDIRS); do \
		echo -e "\033[34mtesting traditional/$$dir\033[0m" && \
		$(MAKE) test -C $$dir TESTER=../$(TESTER); \
	done 

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir TESTER=../$(TESTER); \
	done 
