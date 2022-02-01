.PHONY: all compile test force
all: compile

compile: test.local main.local

# force
test.local: force
	$(CC) test.c -L../ -ljudger_std_io -lkafel -lpthread -static -o test.local -g

main.local: main.c
	$(CC) main.c -o main.local

test: compile
	./test.local main.local main.in main.out main.err && $(MAKE) printlog

printlog:
	@echo "======[log]==============="; \
	cat .log.local; \
	echo "======[log finished]======" \

clean:
	$(RM) test.local

force: ;