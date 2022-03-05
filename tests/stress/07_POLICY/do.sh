#!/bin/bash

rm -f main.out main.err

echo "compiling source..."
clang main.c -o main.local -O2
touch main.out main.err

echo "judging..."
../../../yaoj-judger main.local main.in main.out main.err std \
  -j traditional \
  -r SE \
  --log=.log.local \
  -P . -p invalid \
  -t 1000