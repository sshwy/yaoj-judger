#!/bin/bash

rm -f interactor.out interactor.err main.err

cc main.c -o main.local
cc interactor.c -o interactor.local
touch interactor.out interactor.err main.err

echo -e "Test interactive failure #1"
../../../yaoj-judger main.local interactor.local \
  interactor.in interactor.out i.err main.err \
  -j interactive \
  -r OK \
  --log=.log.local \
  -p builtin:free \
  -t 1000

echo -e "Test interactive failure #2"
../../../yaoj-judger m.local interactor.local \
  interactor.in interactor.out interactor.err main.err \
  -j interactive \
  -r OK \
  --log=.log.local \
  -p builtin:free \
  -t 1000

echo -e "Test interactive failure #3"
../../../yaoj-judger main.local i.local \
  interactor.in interactor.out interactor.err main.err \
  -j interactive \
  -r OK \
  --log=.log.local \
  -p builtin:free \
  -t 1000

echo -e "Test interactive failure #4"
../../../yaoj-judger main.local i.local \
  interactor.in interactor.out interactor.err main.err \
  -j interactive \
  -r OK \
  --log=.log.local \
  -p builtin:free \
  -t 1000 \
  -m -1

echo -e "Test interactive failure #5"
../../../yaoj-judger main.local interactor.local \
  interactor.in interactor.out interactor.err m.err \
  -j interactive \
  -r OK \
  --log=.log.local \
  -p builtin:free \
  -t 1000

echo -e "Test interactive failure #6"
../../../yaoj-judger main.local interactor.local \
  interactor.in interactor.out interactor.err \
  -j interactive \
  -r OK \
  --log=.log.local \
  -p builtin:free \
  -t 1000

echo -e "Test interactive failure #7"
../../../yaoj-judger main.local interactor.local \
  interactor.in interactor.out interactor.err main.err \
  -j interactive \
  -r OK \
  --log=.log.local \
  -p builtin:free \
  -t 1000 \
  --real-memory=-1

echo -e "Test interactive failure #8"
../../../yaoj-judger main.local interactor.local \
  interactor.in interactor.out interactor.err main.err \
  -j interactive \
  -r OK \
  --log=.log.local \
  -p builtin:free \
  -t 1000 \
  --stack-memory=-1

exit 0
