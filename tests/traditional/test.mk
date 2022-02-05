
test: compile
	touch main.out main.err && \
	$(TESTER) -r $(R_CODE) -t $(TIME) -g $(OUTPUT) \
		--virtualmem=$(V_MEM) \
		--actualmem=$(A_MEM) \
		--stackmem=$(S_MEM) \
		main.local main.in main.out main.err $(IO)

compile:
	$(CC) main.c -o main.local

clean:
	$(RM) main.local .log.local main.out main.err