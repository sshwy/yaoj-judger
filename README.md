# Judger of Yao OJ

（大概）是新一代 OJ 评测模块！

虽然没啥新 feature……

不过代码是手撸的，很新！

如何上手？现在还在 prototype 阶段，还没写文档，麻烦您阅读源码……

## 简介

基于 [kafel](https://github.com/google/kafel) 的一个沙箱模块。主要用于 OI/ACM 的代码评测。

## 设计思路

早期思路来源于 [QingdaoU/Judger](https://github.com/QingdaoU/Judger)，在此鸣谢。在此基础上引入 kafel 以更友好的方式配置系统调用规则，并重新整理了执行逻辑。

首先分析一下 OJ 判题的流程。一个题目有若干个测试点，可能有子任务依赖关系，这些都是顶层逻辑。下一层就是在沙箱中执行程序，而执行程序是一个通用的模块，囊括编译、执行和检查等的步骤。执行单个程序的过程中不会有与外界的信息交换，但在执行两个程序之间可以进行信息交换（比如看看当前评测到哪个点了），这部分也可以交给顶层逻辑完成。

而执行一个程序要考虑三个要素：资源限制（时间、空间）、系统安全（系统调用的限制）、数据处理（执行程序获得输出）。

容易发现三个要素互不相关。其中资源限制是高度通用的模块，适用于几乎所有地方。系统调用的限制则需要考虑不同的执行过程以应用不同的 policy，也相对通用。而数据处理的部分则稍显复杂。不同程序的传参不同，并且 IO 的配置也有区别。

不过考虑到数据处理的情况是有限的，目前来看 OI 评测有以下几种代码：

- 标准输入输出型（std_io）: exec + input_file + output_file + error_file
- 文件 IO 型（file_io）: exec + input_file + output_file + error_file
- 交互型（interaction）: exec + interactor + input_file(both) + output_file(interactor) + error_file(both)
- 通用型（代码编译、执行一段命令、testlib）（general）: execve() + input_file + output_file + error_file

那么我们可以针对每种情况分别写 runner，这样 judger 和 runner 模块分开处理。

大致步骤如下：解析 policy => runner 的一些前置操作（prework） => 设置资源限制 => 应用 policy => 执行 runner（run）

## Todo

- 其他 runner 的开发

## Reference

https://zhuanlan.zhihu.com/p/363174561
https://www.conventionalcommits.org/en/v1.0.0/
https://www-uxsup.csx.cam.ac.uk/courses/moved.Building/signals.pdf

## Thanks to

[QingdaoU/Judger](https://github.com/QingdaoU/Judger): [SATA LICENSE](https://github.com/QingdaoU/Judger/raw/newnew/LICENSE)
