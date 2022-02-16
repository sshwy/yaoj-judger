touch main.out main.err

cc main.c -o main.local

echo -e "test failure #1"
../../../judger_traditional.local main.local main.in main.out main.err\
  -r OK \
  -P ../../../policy \
  -p free \
  --log=.log.local \
  -t 1000 -m 128

echo -e "test failure #2"
../../../judger_traditional.local main.local m.in main.out main.err std\
  -r OK \
  -P ../../../policy \
  -p free \
  --log=.log.local \
  -t 1000 -m 128

echo -e "test failure #3"
../../../judger_traditional.local main.local main.in m.out main.err std\
  -r OK \
  -P ../../../policy \
  -p free \
  --log=.log.local \
  -t 1000 -m 128

echo -e "test failure #4"
../../../judger_traditional.local main.local main.in main.out main.err s\
  -r OK \
  -P ../../../policy \
  -p free \
  --log=.log.local \
  -t 1000 -m 128

echo -e "test failure #5"
../../../judger_traditional.local main.local main.in main.out m.err std\
  -r OK \
  -P ../../../policy \
  -p free \
  --log=.log.local \
  -t 1000 -m 128

echo -e "test failure #6"
../../../judger_traditional.local main.local main.in main.out main.err std\
  -r OK \
  -P ../../../policy \
  -p free \
  --log=.log.local \
  -t 1000 -m -128

echo -e "test failure #7"
../../../judger_traditional.local main.local main.in main.err main.err std\
  -r OK \
  -P ../../../policy \
  -p free \
  --log=.log.local \
  -t 1000

exit 0
