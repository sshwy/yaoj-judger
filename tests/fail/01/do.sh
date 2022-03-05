touch main.out main.err

cc main.c -o main.local

echo -e "test failure #1"
../../../yjudger.local main.local main.in main.out main.err\
  -j traditional \
  -r OK \
  -p builtin:free \
  --log=.log.local \
  -t 1000 -m 128

echo -e "test failure #2"
../../../yjudger.local main.local m.in main.out main.err std\
  -j traditional \
  -r OK \
  -p builtin:free \
  --log=.log.local \
  -t 1000 -m 128

echo -e "test failure #3"
../../../yjudger.local main.local main.in m.out main.err std\
  -j traditional \
  -r OK \
  -p builtin:free \
  --log=.log.local \
  -t 1000 -m 128

echo -e "test failure #4"
../../../yjudger.local main.local main.in main.out main.err s\
  -j traditional \
  -r OK \
  -p builtin:free \
  --log=.log.local \
  -t 1000 -m 128

echo -e "test failure #5"
../../../yjudger.local main.local main.in main.out m.err std\
  -j traditional \
  -r OK \
  -p builtin:free \
  --log=.log.local \
  -t 1000 -m 128

echo -e "test failure #6"
../../../yjudger.local main.local main.in main.out main.err std\
  -j traditional \
  -r OK \
  -p builtin:free \
  --log=.log.local \
  -t 1000 -m -128

echo -e "test failure #7"
../../../yjudger.local main.local main.in main.err main.err std\
  -j traditional \
  -r OK \
  -p builtin:free \
  --log=.log.local \
  -t 1000

exit 0
