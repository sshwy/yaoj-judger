# Judger of Yao OJ

（大概）是新一代 OJ 评测模块！

虽然没啥新 feature……

不过代码是手撸的，很新！

## Getting Start

如何上手？现在还在 prototype 阶段，还没写文档，~~麻烦您阅读源码……~~

```bash
make                     # 生成 judger_xxx.local 以及一些链接库
cd tests/traditional/01_DSC   # 测试一下
gcc main.c -o main.local # 编译测试用的代码
touch main.out main.err  # 创建测试代码的 stdout, stderr 对应的文件
../../../judger_traditional.local main.local main.in main.out main.err std \
  -r DSC \
  -P ../../../policy \
  -p c_std_io \
  --log=log.local
cat log.local # 看看评测结果吧
```

另外执行 `./judger_xxx.local --help` 可获得更多玩法。

```
$ ./judger_traditional.local --help
Usage: judger_traditional.local [OPTION...] runner arguments...
Command line interface of judger (for the future yaoj)

      --actmem=ACTUAL_MEMORY specify the actual memory limit in MB
  -g, --fsize=OUTPUT         specify the output limit in MB
      --log=LOG_FILE         specify judger result file
  -m, --memory=MEMORY        specify all three memory limits in MB
  -p, --policy=POLICY        specify policy name
  -P, --policydir=POLICY_DIR specify policy search directory
  -r, --result=RESULT        specify the result code using name
      --stkmem=STACK_MEMORY  specify the stack memory limit in MB
  -t, --timeout=TIMEOUT      specify the time limit in milliseconds
      --virmem=VIRTUAL_MEMORY   specify the virtual memory limit in MB
  -?, --help                 Give this help list
      --usage                Give a short usage message
```

对于更多使用方法，您可以去 [tests/](https://github.com/sshwy/yaoj-judger/tree/master/tests) 了解一下！

## Overview

基于 [kafel](https://github.com/google/kafel) 的一个沙箱模块。主要用于 OI/ACM 的代码评测。

本项目只关注核心：在限制条件下执行某一程序（参数）并得到相应的运行结果分析。

## Design

早期思路来源于 [QingdaoU/Judger](https://github.com/QingdaoU/Judger)，在此鸣谢。在此基础上引入 kafel 以更友好的方式配置系统调用规则，并重新整理了执行逻辑。

首先分析一下 OJ 判题的流程。一个题目有若干个测试点，可能有子任务依赖关系，这些都是顶层逻辑。下一层就是在沙箱中执行程序，而执行程序是一个通用的模块，囊括编译、执行和检查等的步骤。执行单个程序的过程中不会有与外界的信息交换，但在执行两个程序之间可以进行信息交换（比如看看当前评测到哪个点了），这部分也可以交给顶层逻辑完成。

而执行一个程序要考虑三个要素：资源限制（时间、空间）、系统安全（系统调用的限制）、数据处理（执行程序获得输出）。

容易发现三个要素互不相关。其中资源限制是高度通用的模块，适用于几乎所有地方。系统调用的限制则需要考虑不同的执行过程以应用不同的 policy，也相对通用。而数据处理的部分则稍显复杂。不同程序的传参不同，并且 IO 的配置也有区别。

不过考虑到数据处理的情况是有限的，目前来看 OI 评测有以下几种代码：

- 标准输入输出型（std_io）: exec + input_file + output_file + error_file
- 文件 IO 型（file_io）: exec + input_file + output_file + error_file
- 交互型（interaction）: exec + interactor + input_file(both) + output_file(interactor) + error_file(both)
- 通用型（代码编译、执行一段命令、testlib）（general）: execve() + input_file + output_file + error_file

### hooks

为了处理资源限制和系统安全限制，我们引入 hooks 框架（其实也可以理解为一个 event 模式），一定程度上规范了程序的评测过程。简单来说所有对当前进程施加的影响都可以封装为一个模块来调用。而把模块挂载（register）到它该执行的阶段就是 hook。

一个典型的 judger 应该具有以下过程：

![](https://mermaid.ink/img/pako:eNpVkEtuwzAMRK8icJUAcQ-gRVfpCdqlAIOx6FqILRksDSOVffdS_iy6GYjSzDyBGZrkCSx8M46d-bq7GGKQS5GAffilq6mqd_Nou0uX0tM-qE1Mtcrz6qJeb89lzEXWMi4jMkVZDJ4pbIX4DLE31ZuG8Ag3teTcdKH3tbqGEFFo3YoMzv8KZgyiBaVn42yhxTSFsxeMTHOhbGn2OTOhfx1tu4WnuPOVCzcYFInB6wqyi8Y4kI4GcmD16KnFqRcHLq5qnUavX_vwQRKDbbH_oRvgJOnzFRuwwhOdpntA3ehwuNY_z5Z8xg)

除了 child 的部分需要一些逻辑实现，其他部分都可以用 hook 的形式挂载到框架上。原理在于，父进程不会与子进程有通讯，它只需要明确：

- 子进程的较为准确的开始（方便计时）
- 子进程的结束以及其状态码（结果判定）

对于第一个任务，我们采用无名管道的方式，在 child_prework 完成前一直阻塞父进程，直到 ready 信号发出。这样 after_fork 的第一个 hook 执行的时间就非常接近于 child_run 的执行时间。

对于第二个任务，使用 wait 和 getrusage 即可。前者需要逻辑方面的实现，而后者也可以封装为 hook。

### policy

对于系统安全方面的限制，可以想到 seccomp，以及其 bpf 等。本项目采用更加人性化的方式：kafel 规则。它可读且易于扩展。

当然，有一些单纯的 kafel 做不了的需求，例如动态的规则内容。这时我们可以在 kafel 编译前外加一些字符串处理，把规则内容模板化。这样可以动态插入所需内容。

于是，我们将定制化的需求通过魔改 kafel-lang 实现。例如引入 `%[0-9]s` 占位符表示 runner 的参数列表字符串地址。这样可以更好地对 execve 等系统调用进行限制。

已知问题：只能在注册的入口 policy 中使用占位符（没有实现递归替换）。

## Todo

- 其他 judger 的开发
- 同时考虑到实际运行时间可能与系统状态有关，相比之下 cpu 运行时间在 ban 掉一些系统调用后算相对合理的一种衡量方式，因此需要灵活设置
- interactor 错误处理，结果判定（？）等，例如对 executable 的资源占用查看（一般情况下不需要，因为交互题不太关注这个）

## Reference

- [linux 安全模块 -- seccomp 详解](https://zhuanlan.zhihu.com/p/363174561)
- [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/)
- [A list of signals and what they mean](https://www-uxsup.csx.cam.ac.uk/courses/moved.Building/signals.pdf)
- https://opensource.qduoj.com/#/judger/how_it_works
- https://notes.sshwy.name/Interactive-Prob-Judger/

## Thanks to

[QingdaoU/Judger](https://github.com/QingdaoU/Judger): [SATA LICENSE](https://github.com/QingdaoU/Judger/raw/newnew/LICENSE)

## Appendix

```mermaid
graph TD
init(initialize) --> bfh(hook:before_fork)
bfh --> fork{fork} -->|parent| afh(hook:after_fork)
rd -.-> afh --> c_t{{child_terminate}} --> awh(hook:after_wait)
fork -->|child| cafh(child_prework) --> rd{{ready}} --> child_run -.-> c_t
```
