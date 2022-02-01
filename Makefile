SUBDIRS=src policy
PROJECT_ROOT?=

.PHONY: $(SUBDIRS) clean kafel clean_all
all: kafel $(SUBDIRS)
	$(MAKE) -C tests PROJECT_ROOT=../$(PROJECT_ROOT)

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
	$(RM) libkafel.*; \
	$(MAKE) clean -C tests PROJECT_ROOT=../$(PROJECT_ROOT); \
	for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir PROJECT_ROOT=../$(PROJECT_ROOT); \
	done

clean_all: clean
	$(MAKE) clean -C kafel

test:
	$(MAKE) test -C tests