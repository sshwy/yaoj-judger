COMMON_TARGET=judger.o common.o tkill.o
RUNNER_TARGET=runner/std_io.o
ARCHIVE_LIBRARY=$(subst .o,.a,$(subst runner/,libjudger_,$(RUNNER_TARGET)))

.PHONY: all clean

# 对于每个 runner 分别构建链接库
all: $(COMMON_TARGET) $(RUNNER_TARGET)
	$(AR) -r $(PROJECT_ROOT)libjudger_std_io.a runner/std_io.o $(COMMON_TARGET)

clean:
	$(RM) -f $(COMMON_TARGET) $(RUNNER_TARGET) $(addprefix $(PROJECT_ROOT),$(ARCHIVE_LIBRARY)); \

# https://www.gnu.org/software/make/manual/make.html#Implicit-Rules
judger.o: common.h judger.h
common.o: common.h
tkill.o: common.h tkill.h
runner/std_io.o: runner/runner.h common.h