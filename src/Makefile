.PHONY: all runner clean
all: runner

runner: judger.o common.o tkill.o
	$(MAKE) -C runner PROJECT_ROOT=../$(PROJECT_ROOT)

# https://www.gnu.org/software/make/manual/make.html#Implicit-Rules
judger.o: common.h judger.h judger.c
common.o: common.h common.c
tkill.o: common.h tkill.h tkill.c

clean:
	$(RM) -f judger.o common.o; \
	$(MAKE) clean -C runner PROJECT_ROOT=../$(PROJECT_ROOT)