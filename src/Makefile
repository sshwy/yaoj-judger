CFLAGS+=-fPIC -fvisibility=hidden -O2 -Wall -Wextra -Wno-unused-parameter \
	-I$(PROJECT_ROOT)src -I$(PROJECT_ROOT)kafel/include
COMMON_TARGET=\
	common.o\
	helper.o\
	hook.o\
	lib/builtin_hook.o\
	lib/policy.o\
	lib/resouce.o\
	lib/tkill.o\

JUDGER_TARGET=\
	judger/traditional.o\
	judger/interactive.o\
	judger/general.o\

ARCHIVE_LIBRARY=$(subst .o,.a,$(subst judger/,$(PROJECT_ROOT)libjudger_,$(JUDGER_TARGET)))

.PHONY: all clean

# 对于每个 runner 分别构建链接库
all: $(COMMON_TARGET) $(JUDGER_TARGET)
	$(AR) -r $(PROJECT_ROOT)libjudger_traditional.a judger/traditional.o $(COMMON_TARGET); \
	$(AR) -r $(PROJECT_ROOT)libjudger_interactive.a judger/interactive.o $(COMMON_TARGET); \
	$(AR) -r $(PROJECT_ROOT)libjudger_general.a judger/general.o $(COMMON_TARGET); \

clean:
	$(RM) $(COMMON_TARGET) $(JUDGER_TARGET) $(ARCHIVE_LIBRARY)

# https://www.gnu.org/software/make/manual/make.html#Implicit-Rules
common.o: common.h
hook.o: common.h hook.h
judger/interactive.o: common.h judger.h hook.h lib/builtin_hook.h lib/policy.h lib/resouce.h
judger/traditional.o: common.h judger.h hook.h lib/builtin_hook.h lib/policy.h lib/resouce.h
judger/general.o: common.h judger.h hook.h lib/builtin_hook.h lib/policy.h lib/resouce.h
lib/builtin_hook.o: common.h hook.h lib/builtin_hook.h lib/policy.h lib/resouce.h lib/tkill.h
lib/policy.o: common.h lib/policy.h
lib/resouce.o: common.h lib/resouce.h
lib/tkill.o: common.h lib/tkill.h
tackle.o: common.h tackle.h
helper.o: common.h judger.h 