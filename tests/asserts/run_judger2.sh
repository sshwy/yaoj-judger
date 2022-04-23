#!/bin/bash

../yaoj-judger -j interactive --log=$1 guess.local interactor.local asserts/interactor.in \
  t.out /dev/null /dev/null -r OK -p builtin:free
