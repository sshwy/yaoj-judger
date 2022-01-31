
.PHONY: all clean
all: std_io.o
	$(AR) -r $(PROJECT_ROOT)libjudger_std_io.a ../judger.o ../common.o ../tkill.o std_io.o

std_io.o: runner.h ../common.h

clean:
	$(RM) -f $(PROJECT_ROOT)libjudger_std_io.a std_io.o