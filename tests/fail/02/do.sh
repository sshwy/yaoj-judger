#!/bin/bash

echo -e "test failure #1"
../../../judger_general.local $(which ls) ls . \
  -r SE \
  -P ../../../policy \
  -p free \
  -t 1000 -m 128

echo -e "test failure #2"
../../../judger_general.local $(which ls) ls . \
  -r SE \
  --log=.log.local \
  -P ../../../policy \
  -p free \
  -t -1

echo -e "test failure #3"
../../../judger_general.local $(which ls) ls . \
  -r OK \
  --log=.log.local \
  -P ../../../policy \
  -p free \
  -m -128

echo -e "test failure #4"
../../../judger_general.local $(which ls) ls . \
  -r OK \
  --log=.log.local \
  -P ../../../policy \
  -p free \
  -g -128

exit 0