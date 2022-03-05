SHELL=/bin/sh
.PHONY=test compile clean

test: compile
	@$(RM) main.out main.err
	@touch main.out main.err
	@$(PROJECT_ROOT)yjudger.local main.local main.in main.out main.err $(IO) \
		-j traditional \
		-r $(R_CODE) --log=.log.local \
		-p builtin:_coverage \
		-t $(TIME) -g $(OUTPUT) --virtual-memory=$(V_MEM) \
		--real-memory=$(A_MEM) --stack-memory=$(S_MEM) \

compile:
	@$(CC) main.c -o main.local $(EXTRA_FLAG)

clean:
	$(RM) main.local .log.local main.out main.err
