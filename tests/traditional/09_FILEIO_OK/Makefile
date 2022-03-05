include ../../variables.mk
#limitations
TIME=1500
V_MEM=30
A_MEM=30
S_MEM=30
OUTPUT=64
R_CODE=$(OK)
IO=file

SHELL=/bin/sh
.PHONY=test compile clean

test: compile
	@$(RM) main.out main.err
	@touch main.out main.err
	$(PROJECT_ROOT)yaoj-judger main.local main.in main.out main.err $(IO) \
		-j traditional \
		-r $(R_CODE) --log=.log.local \
		-t $(TIME) -g $(OUTPUT) --virtual-memory=$(V_MEM) \
		--real-memory=$(A_MEM) --stack-memory=$(S_MEM) \
		-p builtin:_coverage
#		-P $(PROJECT_ROOT)policy -p cfileio \

compile:
	@$(CC) main.c -o main.local $(EXTRA_FLAG)

clean:
	$(RM) main.local .log.local main.out main.err

