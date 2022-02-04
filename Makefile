SUBDIRS=src policy tests
PROJECT_ROOT?=

.PHONY: $(SUBDIRS) clean kafel clean_all
all: kafel $(SUBDIRS)
	$(CC) main.c -o judger_std_io.local \
		-L./ -ljudger_std_io -lkafel -lpthread -static -I$(PROJECT_ROOT)src -O2 \
		-Wall -Wextra -Wno-missing-field-initializers -Wno-implicit-fallthrough \
		-Wno-nonnull

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