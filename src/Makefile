CFLAGS+=-fPIC -fvisibility=hidden -O2 -Wall -Wextra -Wno-unused-parameter \
	-I$(PROJECT_ROOT)src -I$(PROJECT_ROOT)kafel/include

COMMON_NAME=common helper hook lib/builtin_hook lib/policy lib/resouce lib/tkill
COMMON_TARGET=$(COMMON_NAME:%=%.o)

JUDGER_NAME=traditional interactive general
JUDGER_TARGET=$(JUDGER_NAME:%=judger/%.o)

ARCHIVE_LIBRARY=$(JUDGER_NAME:%=$(PROJECT_ROOT)libjudger_%.a)

.PHONY: all clean $(JUDGER_NAME)

# 对于每个 runner 分别构建链接库
all: $(COMMON_TARGET) $(JUDGER_TARGET) $(JUDGER_NAME)

# build corresponding static library
$(JUDGER_NAME): $(COMMON_TARGET) $(JUDGER_TARGET)
	$(AR) -r $(PROJECT_ROOT)libjudger_$@.a judger/$@.o $(COMMON_TARGET)

clean:
	$(RM) $(COMMON_TARGET) $(JUDGER_TARGET) $(ARCHIVE_LIBRARY)

# https://www.gnu.org/software/make/manual/make.html#Implicit-Rules
common.o: common.h judger.h
helper.o: common.h judger.h lib/policy.h lib/resouce.h hook.h
hook.o: common.h hook.h
judger/general.o: common.h judger.h hook.h lib/builtin_hook.h lib/policy.h lib/resouce.h
judger/interactive.o: common.h judger.h hook.h lib/builtin_hook.h lib/policy.h lib/resouce.h
judger/traditional.o: common.h judger.h hook.h lib/builtin_hook.h lib/policy.h lib/resouce.h
lib/builtin_hook.o: common.h hook.h lib/builtin_hook.h lib/policy.h lib/resouce.h lib/tkill.h
lib/policy.o: common.h lib/policy.h
lib/resouce.o: common.h lib/resouce.h
lib/tkill.o: common.h judger.h lib/tkill.h lib/resouce.h