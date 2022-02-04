CFLAGS+=-fPIC -fvisibility=hidden -O2 -Wall -Wextra -Wno-unused-parameter \
	-I$(PROJECT_ROOT)src -I$(PROJECT_ROOT)kafel/include
COMMON_TARGET=judger.o common.o lib/tkill.o hook.o builtin_hook.o lib/policy.o tackle.o lib/resouce_limit.o
RUNNER_TARGET=runner/std_io.o
ARCHIVE_LIBRARY=$(subst .o,.a,$(subst runner/,libjudger_,$(RUNNER_TARGET)))

.PHONY: all clean

# 对于每个 runner 分别构建链接库
all: $(COMMON_TARGET) $(RUNNER_TARGET)
	$(AR) -r $(PROJECT_ROOT)libjudger_std_io.a runner/std_io.o $(COMMON_TARGET)

clean:
	$(RM) $(COMMON_TARGET) $(RUNNER_TARGET) $(addprefix $(PROJECT_ROOT),$(ARCHIVE_LIBRARY))

# https://www.gnu.org/software/make/manual/make.html#Implicit-Rules
judger.o: common.h judger.h
common.o: common.h
lib/tkill.o: common.h lib/tkill.h
runner/std_io.o: runner.h common.h
hook.o: common.h hook.h
builtin_hook.o: common.h hook.h builtin_hook.h
lib/policy.o: common.h lib/policy.h
lib/resouce_limit.o: common.h lib/resouce_limit.h
tackle.o: common.h tackle.h