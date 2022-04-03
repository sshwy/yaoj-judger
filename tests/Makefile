SHELL=/bin/bash
.PHONY: all test clean force
all:

test:
	./test.sh
	$(MAKE) test -C traditional PROJECT_ROOT=../$(PROJECT_ROOT)

force: ;

clean:
	./test_clean.sh
	$(MAKE) clean -C traditional PROJECT_ROOT=../$(PROJECT_ROOT) -s
