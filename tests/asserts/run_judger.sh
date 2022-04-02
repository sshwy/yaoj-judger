#!/bin/bash
../yaoj-judger asserts/ls.sh \
  -j general \
  -r OK \
  -p builtin:free \
  -t 1000 -m 128 \
  --log=run_judger.log
