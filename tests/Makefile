TESTER=$(PROJECT_ROOT)judger_traditional.local
.PHONY: all test clean
all:

test:
	$(MAKE) test -C traditional TESTER=../$(TESTER)

clean:
	$(RM) test.local && \
	$(MAKE) clean -C traditional TESTER=../$(TESTER) -s