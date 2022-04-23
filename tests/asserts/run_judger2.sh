#!/bin/bash
../yaoj-judger /dev/null /dev/null /dev/null asserts/ls.sh \
  -j general -r OK -p builtin:free -t 1000 --log=$1 #run_judger.log
