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

CLI_CFLAGS=$(COMMON_FLAGS) -lkafel -lpthread -static -I$(PROJECT_ROOT)src

ifeq (coverage,$(MODE)) # if enable coverage
	CLI_CFLAGS+=--coverage -O0
else # production mode
	CLI_CFLAGS+=-O2
endif

COMMON_NAME=common helper hook lib/builtin_hook lib/policy lib/resource lib/tkill
JUDGER_NAME=traditional interactive general

COMMON_TARGET=$(COMMON_NAME:%=%.o)
JUDGER_TARGET=$(JUDGER_NAME:%=judger/%.o)
TARGET=$(COMMON_TARGET) $(JUDGER_TARGET) main.o
GCDA_FILES=$(TARGET:%.o=%.gcda)
# GCOV_FILES=$(TARGET:%.o=%.gcno) $(TARGET:%.o=%.gcda) $(TARGET:%.o=%.c.gcov)
GCOV_FILES=*.gcno *.gcov **/*.gcno **/*.gcov $(GCDA_FILES)
ARCHIVE_LIBRARY=$(JUDGER_NAME:%=$(PROJECT_ROOT)libjudger_%.a)

.PHONY: all clean $(JUDGER_NAME) compile cov

all: compile

# compile source files to object files and then link them together equiping with different judger
compile: $(TARGET) $(JUDGER_NAME)

clean:
	$(RM) $(TARGET) $(ARCHIVE_LIBRARY) $(GCOV_FILES)  

cov:
ifeq (true,$(GCOVR)) # if enable gcovr
	$(GCOV) -abcfu $(GCDA_FILES) && \
	gcovr -r . -k -g --gcov-ignore-parse-errors --html --html-details -o $(PROJECT_ROOT)/local.cov/coverage.html && \
	mv *.c.gcov $(PROJECT_ROOT)local.cov
else # production mode
	$(GCOV) -abcfu $(GCDA_FILES) && \
	mv *.c.gcov $(PROJECT_ROOT)local.cov
endif

# build corresponding static library and cli executable for each judger
$(JUDGER_NAME): $(TARGET)
	$(AR) -r $(PROJECT_ROOT)libjudger_$@.a judger/$@.o $(COMMON_TARGET) && \
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