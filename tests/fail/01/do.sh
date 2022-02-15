#!/bin/bash

../../../judger_general.local $(which ls) ls . \
  -r SE \
  --log=.log.local \
  -t 1000 -m 128
