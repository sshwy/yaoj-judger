#!/bin/bash
rm -f interactor.out interactor.err main.err

cc main.c -o main.local
cc interactor.c -o interactor.local
touch interactor.out interactor.err main.err

../../../judger_interactive.local main.local interactor.local \
  interactor.in interactor.out interactor.err main.err \
  -r TLE \
  --log=.log.local \
  -P ../../../policy \
  -p free \
  -t 500
# cat .log.local
# echo -e "\033[32m[interactor.out]\033[0m"
# cat interactor.out
# echo -e "\033[32m[interactor.err]\033[0m"
# cat interactor.err
# echo -e "\033[32m[main.err]\033[0m"
# cat main.err
