#!/bin/bash

echo -e "test failure #1"
../../../judger_general.local script.sh \
  -r SE \
  -P ../../../policy \
  -p free \
  -t 1000 -m 128

echo -e "test failure #2"
../../../judger_general.local script.sh \
  -r SE \
  --log=.log.local \
  -P ../../../policy \
  -p free \
  -t -1

echo -e "test failure #3"
../../../judger_general.local script.sh \
  -r OK \
  --log=.log.local \
  -P ../../../policy \
  -p free \
  -m -128

echo -e "test failure #4"
../../../judger_general.local script.sh \
  -r OK \
  --log=.log.local \
  -P ../../../policy \
  -p free \
  -g -128

echo -e "test failure #5"
../../../judger_general.local script.sh \
  -r OKK \
  --log=.log.local \
  -P ../../../policy -p free

echo -e "test failure #6"
../../../judger_general.local noexec_script.sh \
  -r OKK \
  --log=.log.local \
  -P ../../../policy -p free

echo -e "test failure #7"
../../../judger_general.local script.sh \
  -r SE \
  --log=.log.local \
  -t 1000 -m 128

echo -e "test failure #8"
../../../judger_general.local script.sh 2 \
  -r SE \
  --log=.log.local \
  -t 1000 -m 128 \
  -P ../../../policy -p free

exit 0
