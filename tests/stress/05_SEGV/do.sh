#!/bin/bash

rm -f main.out main.err

echo "compiling source..."
clang main.c -o main.local -O2
touch main.out main.err

echo "judging..."
../../../yaoj-judger main.local main.in main.out main.err std \
  -j traditional \
  -r RE \
  --log=.log.local \
  -p builtin:free \
  -t 1000

# cat .log.local
# echo -e "\033[32m[main.out]\033[0m"
# cat interactor.out
# echo -e "\033[32m[interactor.err]\033[0m"
# cat interactor.err
# echo -e "\033[32m[main.err]\033[0m"
# cat main.err
