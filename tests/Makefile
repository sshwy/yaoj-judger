SHELL=/bin/bash
.PHONY: all test clean force
all:

test:
	$(MAKE) test -C traditional PROJECT_ROOT=../$(PROJECT_ROOT)
	./test.sh

force: ;

clean:
	$(MAKE) clean -C traditional PROJECT_ROOT=../$(PROJECT_ROOT) -s
