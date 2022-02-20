#!/bin/bash

../../../judger_general.local script.sh \
  -r OK \
  --log=.log.local \
  -P ../../../policy \
  -p free \
  -t 1000 -m 128
