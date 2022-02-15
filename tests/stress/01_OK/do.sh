#!/bin/bash

rm -f main.out main.err

echo "compiling source..."
g++ main.cpp -o main.local -O2
touch main.out main.err

echo "judging..."
../../../judger_traditional.local main.local main.in main.out main.err std \
  -r OK \
  --log=.log.local \
  -P ../../../policy \
  -p c_std_io \
  --memory=128 \
  --timeout=5000

# cat .log.local
# echo -e "\033[32m[main.out]\033[0m"
# cat interactor.out
# echo -e "\033[32m[interactor.err]\033[0m"
# cat interactor.err
# echo -e "\033[32m[main.err]\033[0m"
# cat main.err
