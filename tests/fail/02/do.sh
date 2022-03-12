#!/bin/bash

echo -e "test failure #1"
../../../yaoj-judger script.sh \
  -j general \
  -r SE \
  -p builtin:free \
  -t 1000 -m 128

echo -e "test failure #2"
../../../yaoj-judger script.sh \
  -j general \
  -r SE \
  --log=.log.local \
  -p builtin:free \
  -t -1

echo -e "test failure #3"
../../../yaoj-judger script.sh \
  -j general \
  -r OK \
  --log=.log.local \
  -p builtin:free \
  -m -128

echo -e "test failure #4"
../../../yaoj-judger script.sh \
  -j general \
  -r OK \
  --log=.log.local \
  -p builtin:free \
  -g -128

echo -e "test failure #5"
../../../yaoj-judger script.sh \
  -j general \
  -r OKK \
  --log=.log.local \
  -p builtin:free

echo -e "test failure #6"
../../../yaoj-judger noexec_script.sh \
  -j general \
  -r OKK \
  --log=.log.local \
  -p builtin:free

echo -e "test failure #7"
../../../yaoj-judger script.sh \
  -j general \
  -r SE \
  --log=.log.local \
  -t 1000 -m 128

echo -e "test failure #8"
../../../yaoj-judger script.sh 2 \
  -j general \
  -r SE \
  --log=.log.local \
  -t 1000 -m 128 \
  -p builtin:free

echo -e "test failure #9"
../../../yaoj-judger script.sh \
  -j general \
  -r SE \
  --log=.log.local \
  -t 1000 -m 128 \
  -P . -p fre

echo -e "test failure #10"
../../../yaoj-judger script.sh \
  -j general \
  -r SE \
  --log=.log.local \
  -P . -p invalid \
  -t 1000 -m 128

exit 0
