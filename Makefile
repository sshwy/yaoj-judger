SUBDIRS=src policy tests
PROJECT_ROOT?=
CLI_CFLAG=-lkafel -lpthread -static -I$(PROJECT_ROOT)src -O2 \
	-Wall -Wextra -Wno-missing-field-initializers -Wno-implicit-fallthrough

.PHONY: $(SUBDIRS) clean kafel clean_all
all: kafel $(SUBDIRS)
	$(CC) main.c -o judger_traditional.local -L./ -ljudger_traditional $(CLI_CFLAG); \
	$(CC) main.c -o judger_interactive.local -L./ -ljudger_interactive $(CLI_CFLAG); \

# https://www.gnu.org/software/make/manual/make.html#Overriding
# https://www.gnu.org/software/make/manual/make.html#Multiple-Targets
# $@: https://www.gnu.org/software/make/manual/make.html#Automatic-Variables
# $(VAR) is equivalent to ${VAR}
$(SUBDIRS):
	$(MAKE) -C $@ PROJECT_ROOT=../$(PROJECT_ROOT)

# kafel: build kafel/libkafel.a & kafel/libkafel.so
kafel:
	$(MAKE) -C kafel; \
	cp kafel/lib* ./

clean:
	$(RM) libkafel.* *.local; \
	for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir PROJECT_ROOT=../$(PROJECT_ROOT); \
	done

clean_all: clean
	$(MAKE) clean -C kafel 

test:
	$(MAKE) test -C tests PROJECT_ROOT=../$(PROJECT_ROOT)