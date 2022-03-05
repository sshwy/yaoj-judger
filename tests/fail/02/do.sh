#!/bin/bash

echo -e "test failure #1"
../../../yjudger.local script.sh \
  -j general \
  -r SE \
  -p builtin:free \
  -t 1000 -m 128

echo -e "test failure #2"
../../../yjudger.local script.sh \
  -j general \
  -r SE \
  --log=.log.local \
  -p builtin:free \
  -t -1

echo -e "test failure #3"
../../../yjudger.local script.sh \
  -j general \
  -r OK \
  --log=.log.local \
  -p builtin:free \
  -m -128

echo -e "test failure #4"
../../../yjudger.local script.sh \
  -j general \
  -r OK \
  --log=.log.local \
  -p builtin:free \
  -g -128

echo -e "test failure #5"
../../../yjudger.local script.sh \
  -j general \
  -r OKK \
  --log=.log.local \
  -p builtin:free

echo -e "test failure #6"
../../../yjudger.local noexec_script.sh \
  -j general \
  -r OKK \
  --log=.log.local \
  -p builtin:free

echo -e "test failure #7"
../../../yjudger.local script.sh \
  -j general \
  -r SE \
  --log=.log.local \
  -t 1000 -m 128

echo -e "test failure #8"
../../../yjudger.local script.sh 2 \
  -j general \
  -r SE \
  --log=.log.local \
  -t 1000 -m 128 \
  -p builtin:free

echo -e "test failure #9"
../../../yjudger.local script.sh \
  -j general \
  -r SE \
  --log=.log.local \
  -t 1000 -m 128 \
  -p builtin:free

echo -e "test failure #10"
../../../yjudger.local script.sh \
  -j general \
  -r SE \
  --log=.log.local \
  -t 1000 -m 128 \
  -P . -p fre

exit 0
