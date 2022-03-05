#!/bin/bash

../../../yaoj-judger script.sh \
  -j general \
  -r OK \
  --log=.log.local \
  -P . -p free \
  -t 1000 -m 128 && \
../../../yaoj-judger script.sh \
  -j general \
  -r OK \
  --log=.log.local \
  -p builtin:free \
  -t 1000 -m 128