CFLAGS+=-fPIC -fvisibility=hidden -O2 -Wall -Wextra -Wno-unused-parameter \
	-I$(PROJECT_ROOT)src -I$(PROJECT_ROOT)kafel/include
COMMON_TARGET=\
	common.o\
	hook.o\
	lib/builtin_hook.o\
	lib/policy.o\
	lib/resouce.o\
	lib/tkill.o\

JUDGER_TARGET=judger/traditional.o judger/interactive.o
ARCHIVE_LIBRARY=$(subst .o,.a,$(subst judger/,libjudger_,$(JUDGER_TARGET)))

.PHONY: all clean

# 对于每个 runner 分别构建链接库
all: $(COMMON_TARGET) $(JUDGER_TARGET)
	$(AR) -r $(PROJECT_ROOT)libjudger_traditional.a judger/traditional.o $(COMMON_TARGET); \
	$(AR) -r $(PROJECT_ROOT)libjudger_interactive.a judger/interactive.o $(COMMON_TARGET)

clean:
	$(RM) $(COMMON_TARGET) $(JUDGER_TARGET) $(addprefix $(PROJECT_ROOT),$(ARCHIVE_LIBRARY))

# https://www.gnu.org/software/make/manual/make.html#Implicit-Rules
common.o: common.h
hook.o: common.h hook.h
judger/interactive.o: common.h judger.h hook.h lib/builtin_hook.h lib/policy.h lib/resouce.h runner.h
judger/traditional.o: common.h judger.h hook.h lib/builtin_hook.h lib/policy.h lib/resouce.h runner.h
lib/builtin_hook.o: common.h hook.h lib/builtin_hook.h lib/policy.h lib/resouce.h lib/tkill.h
lib/policy.o: common.h lib/policy.h runner.h
lib/resouce.o: common.h lib/resouce.h
lib/tkill.o: common.h lib/tkill.h
tackle.o: common.h tackle.h