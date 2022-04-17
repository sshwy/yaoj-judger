#!/bin/bash

source testlib.sh

cc asserts/10_dsc.c -o 10_dsc.local -static -lpthread
compile_c asserts/01_dsc.c 01_dsc.local
compile_c asserts/02_ok.c 02_ok.local
compile_c asserts/03_tle.c 03_tle.local
compile_c asserts/04_vmle.c 04_vmle.local
compile_c asserts/05_amle.c 05_amle.local
compile_c asserts/06_smle.c 06_smle.local
compile_c asserts/07_ole.c 07_ole.local
compile_c asserts/08_dsc.c 08_dsc.local
compile_c asserts/09_fileio_ok.c 09_fileio_ok.local
compile_c asserts/aplusb.c aplusb.local
compile_c asserts/cpu_1_7.c cpu_1_7.local
compile_c asserts/exit_1.c exit_1.local
compile_c asserts/fileno_lim.c fileno_lim.local
compile_c asserts/guess_interactive.c guess.local
compile_c asserts/interactor.c interactor.local
compile_c asserts/prlimit.c prlimit.local
compile_c asserts/sigsegv.c sigsegv.local
compile_c asserts/sigusr1.c sigusr1.local
compile_c asserts/sleep_1_7.c sleep_1_7.local
compile_cpp asserts/stress.cpp stress.local

touch t.out t.in
rm -rf .log
mkdir .log

declare_test
run_general t.in t.out /dev/null asserts/ls.sh -r OK -P asserts -p free -t 1000

declare_test
run_general t.in t.out /dev/null asserts/ls.sh -r OK -p builtin:free -t 1000 -m 128

declare_test
run_general t.in t.out /dev/null asserts/ls.sh -p builtin:free -t 1000 -m 128 --json

declare_test
run_general no.in t.out /dev/null asserts/ls.sh -r SE -P asserts -p free -t 1000

declare_test # 5
run_general m.in t.out /dev/null aplusb.local\
  -r SE -p builtin:free -t 1000 -m 128

declare_test

run_general asserts/aplusb.in m.out /dev/null aplusb.local\
  -r SE -p builtin:free -t 1000 -m 128

declare_test
run_general t.in no.out /dev/null asserts/ls.sh -r SE -P asserts -p free -t 1000

declare_test
run_general asserts/aplusb.in t.out m.err aplusb.local\
  -r SE -p builtin:free -t 1000 -m 128

declare_test
run_general asserts/aplusb.in t.out /dev/null aplusb.local\
  -r SE -p builtin:free -t 1000 -m -128

declare_test # 10
run_general asserts/aplusb.in t.out /dev/null 03_tle.local \
	-r TLE -m 30 -t 1000 -g 64 -p builtin:_coverage_general_strict

declare_test
run_general t.in t.out /dev/null asserts/ls.sh -r SE -p builtin:free -t -1

declare_test
run_general t.in t.out /dev/null asserts/ls.sh -r SE -p builtin:free -m -128

declare_test
run_general t.in t.out /dev/null asserts/ls.sh -r SE -p builtin:free -g -128

declare_test
run_general t.in t.out /dev/null asserts/noexec_script.sh -r ECE -p builtin:free

declare_test #15
run_general t.in t.out /dev/null asserts/ls.sh -r SE -t 1000 -m 128 -P asserts -p fre

declare_test
run_general t.in t.out /dev/null asserts/ls.sh -r SE -P asserts -p invalid -t 1000 -m 128

declare_test
run_general t.in t.out /dev/null asserts/run_judger.sh -r ECE -p builtin:free -t 1000 -m 128

declare_test
run_interactive guess.local interactor.local asserts/interactor.in \
  t.out /dev/null /dev/null -r OK -p builtin:free -t 0

declare_test
run_interactive exit_1.local interactor.local asserts/interactor.in \
  t.out /dev/null /dev/null -r TLE -p builtin:free -t 600

declare_test #20
run_interactive guess.local interactor.local asserts/interactor.in \
  t.out i.err /dev/null -r SE -p builtin:free -t 1000

declare_test
run_interactive m.local interactor.local asserts/interactor.in \
  t.out /dev/null /dev/null -r TLE -p builtin:free -t 1000

declare_test
run_interactive guess.local i.local asserts/interactor.in \
  t.out /dev/null /dev/null -r ECE -p builtin:free -t 1000

declare_test
run_interactive guess.local i.local asserts/interactor.in \
  t.out /dev/null /dev/null -r SE -p builtin:free -t 1000 -m -1

declare_test
run_interactive guess.local interactor.local asserts/interactor.in \
  t.out /dev/null m.err -r SE -p builtin:free -t 1000

declare_test #25
run_interactive guess.local interactor.local asserts/interactor.in \
  t.out /dev/null -r SE -p builtin:free -t 1000

declare_test
run_interactive guess.local interactor.local asserts/interactor.in \
  t.out /dev/null /dev/null -r SE -p builtin:free -t 1000 --real-memory=-1

declare_test
run_interactive guess.local interactor.local asserts/interactor.in \
  t.out /dev/null /dev/null -r SE -p builtin:free -t 1000 --stack-memory=-1

declare_test "stress"
run_general asserts/stress.in t.out /dev/null stress.local \
  -r OK -p builtin:_coverage_general_strict --memory=128 --timeout=10000 --cputime=10000

declare_test "stress RE"
run_general asserts/stress.in t.out /dev/null stress.local \
  -r RE -p builtin:free --memory=30 --timeout=10000

declare_test "real time TLE" #30
run_general asserts/aplusb.in t.out /dev/null sleep_1_7.local \
  -r TLE -p builtin:free --memory=30 --timeout=1500

declare_test "CPU time TLE"
run_general asserts/aplusb.in t.out /dev/null cpu_1_7.local \
  -r TLE -p builtin:free --memory=30 --realtime=6000 --cputime=1500

declare_test "segment fault"
run_general asserts/aplusb.in t.out /dev/null sigsegv.local \
  -r RE -p builtin:free -t 1000

declare_test
run_general asserts/aplusb.in t.out /dev/null sigusr1.local \
  -r SE -p builtin:free -t 1000

declare_test
run_general asserts/aplusb.in t.out /dev/null aplusb.local \
  -r SE -P asserts -p invalid -t 1000

declare_test "prlimit" #35
run_general asserts/aplusb.in t.out /dev/null prlimit.local \
  -r ECE -p builtin:_coverage_general_strict -t 1000

declare_test
run_general t.in t.out /dev/null -r SE -p builtin:free -t 1000 -m 128

declare_test
run_general t.in t.out no.err asserts/ls.sh -r SE -P asserts -p free -t 1000

declare_test
run_general asserts/aplusb.in t.out /dev/null 05_amle.local \
	-r MLE -m 30 -t 1000 -g 64 -p builtin:_coverage_general_strict \
  --virtual-memory=128 --real-memory=3

declare_test
run_general asserts/aplusb.in t.out /dev/null 06_smle.local \
	-r MLE -m 30 -t 1000 -g 64 -p builtin:_coverage_general_strict \
  --stack-memory=3

declare_test # 40
run_general asserts/aplusb.in t.out /dev/null 04_vmle.local \
	-r MLE -m 30 -t 1000 -g 64 -p builtin:_coverage_general_strict

declare_test
run_general asserts/aplusb.in t.out /dev/null 01_dsc.local \
	-r DSC -m 30 -t 1000 -g 64 -p builtin:_coverage_general_strict 

declare_test
run_general asserts/aplusb.in t.out /dev/null 02_ok.local \
	-r OK -m 30 -t 1000 -g 64 -p builtin:_coverage_general_strict

declare_test
run_general asserts/aplusb.in t.out /dev/null 08_dsc.local \
	-r DSC -m 30 -t 1000 -g 64 -p builtin:_coverage_general_strict 

declare_test
run_general asserts/aplusb.in t.out /dev/null 10_dsc.local \
	-r DSC -m 30 -t 1000 -g 64 -p builtin:_coverage_general_strict 

declare_test #45
run_general /dev/null /dev/null /dev/null 09_fileio_ok.local \
	-r OK -m 30 -t 1000 -g 64 -p builtin:_coverage_general_strict 

declare_test
run_general asserts/aplusb.in t.out /dev/null 07_ole.local \
	-r OLE -m 30 -t 3000 -g 25 -p builtin:_coverage_general_strict 

declare_test "fileno"
run_general /dev/null t.out /dev/null fileno_lim.local \
	-r ECE -m 30 -t 3000 -g 25 -f 5 -p builtin:free

test_finish