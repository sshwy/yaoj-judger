#!/bin/bash

test_count=0
fail_test=
exit_code=0

function declare_test {
  test_count=$((test_count + 1))
  printf "\033[32mRun Test #${test_count}\033[0m $1\n"
}

function declare_test_failed {
  printf "\033[31mRun Test #${test_count} failed\033[0m\n"
}

function declare_test_log {
  printf "\033[33mTest Log #${test_count}\033[0m\n"
}
function declare_test_stderr {
  printf "\033[33mTest Stderr #${test_count}\033[0m\n"
}

function run_judger {
  retry=3
  ret=
  while [ $retry -ne 0 ]; do
    retry=$((retry - 1))
    ../yaoj-judger $@ > .log/${test_count}.out  2> .log/${test_count}.err
    ret=$?
    if [ $ret -eq 0 ]; then
      break
    fi
  done

  if [ $ret -ne 0 ]; then
    declare_test_failed
    strace -o .log/${test_count}.trace -ff ../yaoj-judger $@
    declare_test_log
    cat < .log/${test_count}.log
    declare_test_stderr
    cat < .log/${test_count}.err
    fail_test+=" #${test_count}"
    exit_code=1
    # exit $ret
  fi
}

function run_general {
  run_judger -j general \
    --log=.log/${test_count}.log \
    $@
}

function run_traditional {
  run_judger -j traditional \
    --log=.log/${test_count}.log \
    $@
}

function run_interactive {
  run_judger -j interactive \
    --log=.log/${test_count}.log \
    $@
}

function compile_c {
  cc $1 -o $2
}

function compile_cpp {
  clang++ $1 -o $2 -O2
}

function test_finish {
  if [ $exit_code -ne 0 ]; then
    printf "\033[31mFailed Test: $fail_test\033[0m\n"
    exit $exit_code
  else
    printf "\033[32mAll Test Passed\033[0m\n"
    exit 0
  fi
}