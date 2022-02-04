TESTER=test.local
.PHONY: all compile test force
all: compile

compile: test.local

# force
test.local: force
	$(CC) test.c -L../ -ljudger_std_io -lkafel -lpthread -static -o test.local \
		-I$(PROJECT_ROOT)src -I$(PROJECT_ROOT)kafel/include

test: compile
	$(MAKE) test -C std_io TESTER=../$(TESTER)

printlog:
	@echo "======[log]==============="; \
	cat .log.local; \
	echo "======[log finished]======" \

clean:
	$(RM) test.local && \
	$(MAKE) clean -C std_io TESTER=../$(TESTER) -s

force: ;