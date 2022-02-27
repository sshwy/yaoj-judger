SHELL=/bin/sh
CC=clang
SUBDIRS=policy src tests
PROJECT_ROOT?=

.PHONY: $(SUBDIRS) clean kafel clean_all docs check

all: kafel $(SUBDIRS)
	@echo -e "done."

# https://www.gnu.org/software/make/manual/make.html#Overriding
# https://www.gnu.org/software/make/manual/make.html#Multiple-Targets
# $@: https://www.gnu.org/software/make/manual/make.html#Automatic-Variables
# $(VAR) is equivalent to ${VAR}
$(SUBDIRS):
	$(MAKE) -C $@ PROJECT_ROOT=../$(PROJECT_ROOT)

# kafel: build kafel/libkafel.a & kafel/libkafel.so
kafel:
	$(MAKE) -C kafel
#	cp kafel/lib* ./

clean:
	$(RM) libkafel.* *.local *.gcno *.gcda *.gcov
	$(RM) -r local.cov
	@for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir PROJECT_ROOT=../$(PROJECT_ROOT); \
	done

clean_all: clean
	$(MAKE) clean -C kafel 

check: test

test:
	$(MAKE) test -C tests PROJECT_ROOT=../$(PROJECT_ROOT)

gcovr:
	$(MAKE) clean
	$(MAKE) coverage GCOVR=true

# generate gcov files in local.cov dir
coverage: kafel policy
	$(MAKE) all -C src PROJECT_ROOT=../$(PROJECT_ROOT) MODE=coverage
	$(MAKE) test
	mkdir -p local.cov
	$(MAKE) cov -C src PROJECT_ROOT=../$(PROJECT_ROOT)

docs:
	doxygen; \
	sed -i 's/code.JavaDocCode/code.JavaDocCode{/g' docs/web/html/doxygen.css

src: kafel

# sed is for known bugs, which will be fixed in doxgen 1.9.4