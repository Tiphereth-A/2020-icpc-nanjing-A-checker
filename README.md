# 2020-icpc-nanjing-A-checker

[2020ICPC南京区域赛A题](https://ac.nowcoder.com/acm/contest/10272/A) 的checker

## 如何使用

按需求更改 `namespace _Config` 内的数据, 之后编译并运行 `checker.cpp` 即可
| 常量 / 宏             | 含义                                           | 备注                         |
| :-------------------- | :--------------------------------------------- | :--------------------------- |
| `__STAT_DETAIL__`     | 是否输出详细信息 (包括所有 hack 的步数)        | 输出到 `stdout` 里           |
| `__REC_TIME__`        | 是否输出运行时间                               | 输出到 `stderr` 里           |
| `__PRINT_HACK_DATA__` | 是否输出生成的操作序列                         | 输出到 `stderr` 里           |
| `MAP_MAX`             | 地图大小                                       | 务必确保和测试地图的尺寸一致 |
| `SEQ_MAX`             | 用于 Hack 的操作序列的长度                     |                              |
| `DATA_NUM`            | Hack 的次数                                    |                              |
| `ACCESSABLE_HACK_LEN` | 当执行操作序列到达多少步时认为 Hack 成功       | 务必确保大于 `SEQ_MAX`       |
| `ACCESSABLE_RATE`     | 当成功 Hack 数占总 Hack 数的比率超过多少时通过 |                              |
| `maps`                | 要测试的地图                                   | 只支持方阵                   |

最后结果会输出到 `stdout` 里
