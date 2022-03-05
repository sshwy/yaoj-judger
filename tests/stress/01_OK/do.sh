#!/bin/bash

rm -f main.out main.err

echo "compiling source..."
clang++ main.cpp -o main.local -O2
touch main.out main.err

echo "judging..."
../../../yjudger.local main.local main.in main.out main.err std \
  -j traditional \
  -r OK \
  --log=.log.local \
  -p builtin:_coverage \
  --memory=128 \
  --timeout=5000

# cat .log.local
# echo -e "\033[32m[main.out]\033[0m"
# cat interactor.out
# echo -e "\033[32m[interactor.err]\033[0m"
# cat interactor.err
# echo -e "\033[32m[main.err]\033[0m"
# cat main.err
