SHELL=/bin/sh
GCOV=llvm-cov gcov
CC=clang
COMMON_FLAGS=-g --std=gnu17 -Wall -Wextra

CFLAGS+=$(COMMON_FLAGS) -Wno-unused-parameter -Wno-missing-field-initializers \
	-I$(PROJECT_ROOT)src -I$(PROJECT_ROOT)kafel/include

# https://stackoverflow.com/questions/5311515/gcc-fpic-option
ifeq (coverage,$(MODE)) # if enable coverage
	CFLAGS+=--coverage -O0
else # production mode
	CFLAGS+=-fPIC -fvisibility=hidden -O2
endif

CLI_CFLAGS=$(COMMON_FLAGS) -lpthread -static -I$(PROJECT_ROOT)src

ifeq (coverage,$(MODE)) # if enable coverage
	CLI_CFLAGS+=--coverage -O0
else # production mode
	CLI_CFLAGS+=-O2
endif

COMMON_NAME=common helper hook yerr\
	lib/builtin_hook lib/policy lib/resource lib/tkill lib/builtin_policy.gen
JUDGER_NAME=traditional interactive general

LIB_OBJECTS=$(COMMON_NAME:%=%.o) $(JUDGER_NAME:%=judger/%.o)
GCDA_FILES=$(LIB_OBJECTS:%.o=%.gcda)
GCOV_FILES=$(LIB_OBJECTS:%.o=%.gcno) $(GCDA_FILES)

STATIC_TARGET=$(PROJECT_ROOT)libyjudger.a
EXECUTABLE=$(PROJECT_ROOT)yjudger.local

.PHONY: all clean compile cov builtin_policy

all: compile

# compile source files to object files and then link them together equiping with different judger
compile: $(STATIC_TARGET) $(EXECUTABLE)

$(STATIC_TARGET): builtin_policy $(LIB_OBJECTS)
	$(LD) $(LDFLAGS) -r ${LIB_OBJECTS} -o libyjudger.o
	$(AR) -x $(PROJECT_ROOT)kafel/libkafel.a
	$(AR) -rcs $(STATIC_TARGET) libyjudger.o libkafel.o

$(EXECUTABLE): $(STATIC_TARGET) cli/main.o cli/cli.o
	$(CC) cli/main.o cli/cli.o -o $(EXECUTABLE) -L./$(PROJECT_ROOT) -lyjudger $(CLI_CFLAGS)

clean:
	$(RM) $(LIB_OBJECTS) $(STATIC_TARGET) $(GCOV_FILES) libyjudger.o libkafel.o \
	cli/main.o cli/cli.o cli/main.gcda cli/main.gcno cli/cli.gcda cli/cli.gcno

cov:
	$(GCOV) -abcfu $(GCDA_FILES)
	$(RM) *.gen.c.gcov
ifeq (true,$(GCOVR)) # if enable gcovr
	gcovr -r . -k -g --gcov-ignore-parse-errors --html --html-details -s\
		-o $(PROJECT_ROOT)/local.cov/coverage.html
endif
	mv *.c.gcov $(PROJECT_ROOT)local.cov

cli/cli.c: cli/cli.ggo
	cd cli && gengetopt < cli.ggo && cd ..

builtin_policy:
	$(MAKE) all -C builtin_policy

# https://www.gnu.org/software/make/manual/make.html#Implicit-Rules
cli/cli.o: cli/cli.h
cli/main.o: cli/cli.h judger.h
common.o: common.h judger.h
helper.o: common.h judger.h lib/policy.h lib/resource.h hook.h
hook.o: common.h hook.h
judger/general.o: common.h judger.h hook.h lib/builtin_hook.h lib/policy.h lib/resource.h
judger/interactive.o: common.h judger.h hook.h lib/builtin_hook.h lib/policy.h lib/resource.h
judger/traditional.o: common.h judger.h hook.h lib/builtin_hook.h lib/policy.h lib/resource.h
lib/builtin_hook.o: common.h hook.h lib/builtin_hook.h lib/policy.h lib/resource.h lib/tkill.h
lib/builtin_policy.gen.o: common.h lib/policy.h
lib/policy.o: common.h lib/policy.h
lib/resource.o: common.h lib/resource.h
lib/tkill.o: common.h judger.h lib/tkill.h lib/resource.h
yerr.o: yerr.h