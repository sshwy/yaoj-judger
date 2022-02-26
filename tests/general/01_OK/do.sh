#!/bin/bash

../../../yjudger.local script.sh \
  -j general \
  -r OK \
  --log=.log.local \
  -P ../../../policy \
  -p free \
  -t 1000 -m 128
