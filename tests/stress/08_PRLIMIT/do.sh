#!/bin/bash

rm -f main.out main.err 
rm main.log.local.*

echo "compiling source..."
clang main.c -o main.local -O2 -static
touch main.out main.err

echo "judging..."
strace -o main.log.local -ff ../../../yaoj-judger main.local main.in main.out main.err std \
  -j traditional \
  -r ECE \
  --log=.log.local \
  -p builtin:_coverage \
  -t 1000
