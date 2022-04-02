#!/bin/bash

test_count=0

function declare_test {
  test_count=$((test_count + 1))
  printf "\033[32mRun Test #${test_count}\033[0m $1\n"
}

function declare_test_failed {
  printf "\033[31mRun Test #${test_count} failed\033[0m\n"
}

function run_judger {
  ../yaoj-judger $@
  ret=$?
  if [ $ret -ne 0 ]; then
    declare_test_failed
    cat < .log/${test_count}.log
    exit $ret
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