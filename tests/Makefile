TESTER=$(PROJECT_ROOT)judger_traditional.local
.PHONY: all test clean
all:

test:
	$(MAKE) test -C traditional TESTER=../$(TESTER) PROJECT_ROOT=../$(PROJECT_ROOT) && \
	$(MAKE) test -C interactive TESTER=../$(TESTER) PROJECT_ROOT=../$(PROJECT_ROOT)

clean:
	$(RM) test.local && \
	$(MAKE) clean -C traditional TESTER=../$(TESTER) PROJECT_ROOT=../$(PROJECT_ROOT) -s