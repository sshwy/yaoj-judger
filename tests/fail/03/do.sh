#!/bin/bash

echo -e "test failure #1"
../../../yaoj-judger script.sh \
  -j general \
  -r ECE \
  -p builtin:free \
  --log=.log.local \
  -t 1000 -m 128 && echo "ok"
