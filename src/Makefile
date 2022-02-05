CFLAGS+=-fPIC -fvisibility=hidden -O2 -Wall -Wextra -Wno-unused-parameter \
	-I$(PROJECT_ROOT)src -I$(PROJECT_ROOT)kafel/include
COMMON_TARGET=\
	lib/builtin_hook.o\
	common.o\
	hook.o\
	judger/traditional.o\
	lib/policy.o\
	lib/resouce.o\
	lib/tkill.o\
	tackle.o\

RUNNER_TARGET=runner/std_io.o
ARCHIVE_LIBRARY=$(subst .o,.a,$(subst runner/,libjudger_,$(RUNNER_TARGET)))

.PHONY: all clean

# 对于每个 runner 分别构建链接库
all: $(COMMON_TARGET) $(RUNNER_TARGET)
	$(AR) -r $(PROJECT_ROOT)libjudger_std_io.a runner/std_io.o $(COMMON_TARGET)

clean:
	$(RM) $(COMMON_TARGET) $(RUNNER_TARGET) $(addprefix $(PROJECT_ROOT),$(ARCHIVE_LIBRARY))

# https://www.gnu.org/software/make/manual/make.html#Implicit-Rules
lib/builtin_hook.o: common.h hook.h lib/builtin_hook.h lib/policy.h lib/resouce.h lib/tkill.h
common.o: common.h
hook.o: common.h hook.h
judger/traditional.o: common.h judger.h
lib/policy.o: common.h lib/policy.h runner.h
lib/resouce.o: common.h lib/resouce.h
lib/tkill.o: common.h lib/tkill.h
runner/std_io.o: runner.h common.h
tackle.o: common.h tackle.h