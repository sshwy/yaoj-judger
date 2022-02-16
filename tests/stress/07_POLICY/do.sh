#!/bin/bash

rm -f main.out main.err

echo "compiling source..."
clang main.c -o main.local -O2
touch main.out main.err

echo "judging..."
../../../judger_traditional.local main.local main.in main.out main.err std \
  -r SE \
  --log=.log.local \
  -P . -p invalid \
  -t 1000

[ $? -eq 1 ] && exit 0 || exit 1 # ensure return 1