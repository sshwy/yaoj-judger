SHELL=/bin/sh
CC=clang
.PHONY: clean force

# do it for each run
# write all available system call definition into syscall.policy
syscall.policy: force
	$(CC) update_syscall.c -o update_syscall.local
	./update_syscall.local syscall.polib
	./gen_builtin.sh

clean:
	$(RM) update_syscall.local

force: ;