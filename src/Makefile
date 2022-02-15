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

COMMON_NAME=common helper hook lib/builtin_hook lib/policy lib/resource lib/tkill
JUDGER_NAME=traditional interactive general

COMMON_OBJECTS=$(COMMON_NAME:%=%.o)
JUDGER_OBJECTS=$(JUDGER_NAME:%=judger/%.o)
OBJECTS=$(COMMON_OBJECTS) $(JUDGER_OBJECTS) main.o
GCDA_FILES=$(OBJECTS:%.o=%.gcda)
GCOV_FILES=$(OBJECTS:%.o=%.gcno) $(GCDA_FILES)
ARCHIVE_LIBRARY=$(JUDGER_NAME:%=$(PROJECT_ROOT)libjudger_%.a)

.PHONY: all clean $(JUDGER_NAME) compile cov

all: compile

# compile source files to object files and then link them together equiping with different judger
compile: $(OBJECTS) $(JUDGER_NAME)

clean:
	$(RM) $(OBJECTS) $(ARCHIVE_LIBRARY) $(GCOV_FILES) $(JUDGER_NAME:%=libjudger_%.o) libkafel.o

cov:
ifeq (true,$(GCOVR)) # if enable gcovr
	$(GCOV) -abcfu $(GCDA_FILES)
	gcovr -r . -k -g --gcov-ignore-parse-errors --html --html-details -o $(PROJECT_ROOT)/local.cov/coverage.html
	mv *.c.gcov $(PROJECT_ROOT)local.cov
else # production mode
	$(GCOV) -abcfu $(GCDA_FILES)
	mv *.c.gcov $(PROJECT_ROOT)local.cov
endif

# build corresponding static library and cli executable for each judger
$(JUDGER_NAME): $(OBJECTS)
	$(LD) $(LDFLAGS) -r ${COMMON_OBJECTS} judger/$@.o -o libjudger_$@.o
	$(AR) -x $(PROJECT_ROOT)kafel/libkafel.a
	$(AR) -rcs $(PROJECT_ROOT)libjudger_$@.a libjudger_$@.o libkafel.o
	$(CC) main.o -o $(PROJECT_ROOT)judger_$@.local -L./$(PROJECT_ROOT) -ljudger_$@ $(CLI_CFLAGS)

# https://www.gnu.org/software/make/manual/make.html#Implicit-Rules
common.o: common.h judger.h
helper.o: common.h judger.h lib/policy.h lib/resource.h hook.h
hook.o: common.h hook.h
judger/general.o: common.h judger.h hook.h lib/builtin_hook.h lib/policy.h lib/resource.h
judger/interactive.o: common.h judger.h hook.h lib/builtin_hook.h lib/policy.h lib/resource.h
judger/traditional.o: common.h judger.h hook.h lib/builtin_hook.h lib/policy.h lib/resource.h
lib/builtin_hook.o: common.h hook.h lib/builtin_hook.h lib/policy.h lib/resource.h lib/tkill.h
lib/policy.o: common.h lib/policy.h
lib/resource.o: common.h lib/resource.h
lib/tkill.o: common.h judger.h lib/tkill.h lib/resource.h
main.o: main.c judger.h