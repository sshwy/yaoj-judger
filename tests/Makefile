TESTER=$(PROJECT_ROOT)judger_traditional.local
SUBDIRS=traditional interactive general fail
.PHONY: all test clean force
all:

test: $(SUBDIRS)

$(SUBDIRS): force
	$(MAKE) test -C $@ TESTER=../$(TESTER) PROJECT_ROOT=../$(PROJECT_ROOT)

force: ;

clean:
	$(RM) test.local && \
	$(MAKE) clean -C traditional TESTER=../$(TESTER) PROJECT_ROOT=../$(PROJECT_ROOT) -s