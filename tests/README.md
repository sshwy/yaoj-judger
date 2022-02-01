# Test

存放测试的地方。

目的是检测 policy 的正确性。

简单来说就是运行各种代码看是否达到预期结果。

要进行测试，请在**项目根目录**下执行

```bash
make
make test
```

如果你觉得 `make test` 输出的信息太吵了，也可以用

```bash
make test -s
```

另外在执行了 `make` 之后本目录下会出现一个 `test.local` 的可执行文件，你可以通过 `./test.local --help` 了解用法。
