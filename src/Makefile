CFLAGS+=-fPIC -fvisibility=hidden -O2 -Wall -Wextra -Wno-unused-parameter \
	-I$(PROJECT_ROOT)src -I$(PROJECT_ROOT)kafel/include
COMMON_TARGET=\
	builtin_hook.o\
	child_proc/traditional.o\
	common.o\
	hook.o\
	judger.o\
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
builtin_hook.o: common.h hook.h builtin_hook.h lib/policy.h lib/resouce.h lib/tkill.h
child_proc/traditional.o: common.h runner.h child_proc.h lib/policy.h lib/resouce.h
common.o: common.h
hook.o: common.h hook.h
judger.o: common.h judger.h
lib/policy.o: common.h lib/policy.h runner.h
lib/resouce.o: common.h lib/resouce.h
lib/tkill.o: common.h lib/tkill.h
runner/std_io.o: runner.h common.h
tackle.o: common.h tackle.h