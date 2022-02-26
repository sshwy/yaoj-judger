SHELL=/bin/sh
SUBDIRS=traditional interactive general fail stress
.PHONY: all test clean force
all:

test: $(SUBDIRS)

$(SUBDIRS): force
	$(MAKE) test -C $@ PROJECT_ROOT=../$(PROJECT_ROOT)

force: ;

clean:
	$(RM) test.local
	$(MAKE) clean -C traditional PROJECT_ROOT=../$(PROJECT_ROOT) -s
	$(MAKE) clean -C stress PROJECT_ROOT=../$(PROJECT_ROOT) -s
	$(MAKE) clean -C interactive PROJECT_ROOT=../$(PROJECT_ROOT) -s
	$(MAKE) clean -C fail PROJECT_ROOT=../$(PROJECT_ROOT) -s
	$(MAKE) clean -C general PROJECT_ROOT=../$(PROJECT_ROOT) -s