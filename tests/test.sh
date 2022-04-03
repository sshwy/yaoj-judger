#!/bin/bash

source testlib.sh

compile_c asserts/aplusb.c aplusb.local
compile_c asserts/interactor.c interactor.local
compile_c asserts/guess_interactive.c guess.local
compile_c asserts/exit_1.c exit_1.local
compile_c asserts/sleep_1_7.c sleep_1_7.local
compile_c asserts/cpu_1_7.c cpu_1_7.local
compile_c asserts/sigsegv.c sigsegv.local
compile_c asserts/sigusr1.c sigusr1.local
compile_c asserts/prlimit.c prlimit.local
compile_cpp asserts/stress.cpp stress.local

touch t.out t.err t2.err
rm -rf .log
mkdir .log

declare_test
run_general asserts/ls.sh -r OK -P asserts -p free -t 1000

declare_test
run_general asserts/ls.sh -r OK -p builtin:free -t 1000 -m 128

declare_test
run_general asserts/ls.sh -p builtin:free -t 1000 -m 128 --json

declare_test
run_traditional aplusb.local asserts/aplusb.in t.out t.err\
  -r SE -p builtin:free -t 1000 -m 128

declare_test # 5
run_traditional aplusb.local m.in t.out t.err std\
  -r SE -p builtin:free -t 1000 -m 128

declare_test

run_traditional aplusb.local asserts/aplusb.in m.out t.err std\
  -r SE -p builtin:free -t 1000 -m 128

declare_test
run_traditional aplusb.local asserts/aplusb.in t.out t.err s\
  -r SE -p builtin:free -t 1000 -m 128

declare_test
run_traditional aplusb.local asserts/aplusb.in t.out m.err std\
  -r SE -p builtin:free -t 1000 -m 128

declare_test
run_traditional aplusb.local asserts/aplusb.in t.out t.err std\
  -r SE -p builtin:free -t 1000 -m -128

declare_test # 10
run_traditional aplusb.local asserts/aplusb.in t.err t.err std\
  -r SE -p builtin:free -t 1000

declare_test
run_general asserts/ls.sh -r SE -p builtin:free -t -1

declare_test
run_general asserts/ls.sh -r SE -p builtin:free -m -128

declare_test
run_general asserts/ls.sh -r SE -p builtin:free -g -128

declare_test
run_general asserts/noexec_script.sh -r ECE -p builtin:free

declare_test #15
run_general asserts/ls.sh -r SE -t 1000 -m 128 -P asserts -p fre

declare_test
run_general asserts/ls.sh -r SE -P asserts -p invalid -t 1000 -m 128

declare_test
run_general asserts/run_judger.sh -r ECE -p builtin:free -t 1000 -m 128

declare_test
run_interactive guess.local interactor.local asserts/interactor.in \
  t.out t.err t2.err -r OK -p builtin:free -t 0

declare_test
run_interactive exit_1.local interactor.local asserts/interactor.in \
  t.out t.err t2.err -r TLE -p builtin:free -t 600

declare_test #20
run_interactive guess.local interactor.local asserts/interactor.in \
  t.out i.err t2.err -r SE -p builtin:free -t 1000

declare_test
run_interactive m.local interactor.local asserts/interactor.in \
  t.out t.err t2.err -r TLE -p builtin:free -t 1000

declare_test
run_interactive guess.local i.local asserts/interactor.in \
  t.out t.err t2.err -r ECE -p builtin:free -t 1000

declare_test
run_interactive guess.local i.local asserts/interactor.in \
  t.out t.err t2.err -r SE -p builtin:free -t 1000 -m -1

declare_test
run_interactive guess.local interactor.local asserts/interactor.in \
  t.out t.err m.err -r SE -p builtin:free -t 1000

declare_test #25
run_interactive guess.local interactor.local asserts/interactor.in \
  t.out t.err -r SE -p builtin:free -t 1000

declare_test
run_interactive guess.local interactor.local asserts/interactor.in \
  t.out t.err t2.err -r SE -p builtin:free -t 1000 --real-memory=-1

declare_test
run_interactive guess.local interactor.local asserts/interactor.in \
  t.out t.err t2.err -r SE -p builtin:free -t 1000 --stack-memory=-1

declare_test "stress"

run_traditional stress.local asserts/stress.in t.out t.err std \
  -r OK -p builtin:_coverage --memory=128 --timeout=10000

declare_test "stress RE"
run_traditional stress.local asserts/stress.in t.out t.err std \
  -r RE -p builtin:free --memory=30 --timeout=10000

declare_test "real time TLE" #30
run_traditional sleep_1_7.local asserts/aplusb.in t.out t.err std \
  -r TLE -p builtin:free --memory=30 --timeout=1500

declare_test "CPU time TLE"
run_traditional cpu_1_7.local asserts/aplusb.in t.out t.err std \
  -r TLE -p builtin:free --memory=30 --realtime=6000 --cputime=1500

declare_test "segment fault"
run_traditional sigsegv.local asserts/aplusb.in t.out t.err std \
  -r RE -p builtin:free -t 1000

declare_test
run_traditional sigusr1.local asserts/aplusb.in t.out t.err std \
  -r SE -p builtin:free -t 1000

declare_test
run_traditional aplusb.local asserts/aplusb.in t.out t.err std \
  -r SE -P asserts -p invalid -t 1000

declare_test "prlimit" #35
run_traditional prlimit.local asserts/aplusb.in t.out t.err std \
  -r ECE -p builtin:_coverage -t 1000

test_finish