SHELL=/bin/sh
.PHONY=test compile clean

test: compile
	$(RM) main.out main.err
	touch main.out main.err
	$(TESTER) main.local main.in main.out main.err $(IO) \
		-r $(R_CODE) --log=.log.local \
		-P $(PROJECT_ROOT)policy -p c_std_io_coverage \
		-t $(TIME) -g $(OUTPUT) --virmem=$(V_MEM) \
		--actmem=$(A_MEM) --stkmem=$(S_MEM) \

compile:
	$(CC) main.c -o main.local $(EXTRA_FLAG)

clean:
	$(RM) main.local .log.local main.out main.err
