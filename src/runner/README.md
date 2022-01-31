# runner

一个 runner 包括两个部分：预处理（prework）和执行（run）

runner 不考虑资源限制和 policy。

对于需要在应用 policy 之前完成的工作放在 prework 里进行。

理论上编译的时候链接不同的 runner 可以得到不同的 judger
