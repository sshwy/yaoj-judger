#!/bin/bash

rm -f interactor.out interactor.err main.err

cc main.c -o main.local
cc interactor.c -o interactor.local
touch interactor.out interactor.err main.err

echo -e "Test interactive failure #1"
../../../judger_interactive.local main.local interactor.local \
  interactor.in interactor.out i.err main.err \
  -r OK \
  --log=.log.local \
  -P ../../../policy \
  -p free \
  -t 1000

echo -e "Test interactive failure #2"
../../../judger_interactive.local m.local interactor.local \
  interactor.in interactor.out interactor.err main.err \
  -r OK \
  --log=.log.local \
  -P ../../../policy \
  -p free \
  -t 1000

echo -e "Test interactive failure #3"
../../../judger_interactive.local main.local i.local \
  interactor.in interactor.out interactor.err main.err \
  -r OK \
  --log=.log.local \
  -P ../../../policy \
  -p free \
  -t 1000

echo -e "Test interactive failure #4"
../../../judger_interactive.local main.local i.local \
  interactor.in interactor.out interactor.err main.err \
  -r OK \
  --log=.log.local \
  -P ../../../policy \
  -p free \
  -t 1000 \
  -m -1

echo -e "Test interactive failure #5"
../../../judger_interactive.local main.local interactor.local \
  interactor.in interactor.out interactor.err m.err \
  -r OK \
  --log=.log.local \
  -P ../../../policy \
  -p free \
  -t 1000

echo -e "Test interactive failure #6"
../../../judger_interactive.local main.local interactor.local \
  interactor.in interactor.out interactor.err \
  -r OK \
  --log=.log.local \
  -P ../../../policy \
  -p free \
  -t 1000

exit 0
