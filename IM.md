# IM 海量聊天消息序列生成

## 如何确保聊天消息的唯一性和顺序性

- 看到这个问题，普遍考虑的是用自增序列作为消息 ID ，这样既可以保证唯一性，又可以保证顺序性
但是在分布式场景下很难保证 id 的唯一性和顺序递增，维护 id 生成的一致性难度又很大

## 项目当中的解决方案

- Twitter 的 snowflake 算法

  - snowflake 是 Twitter 开源的分布式 ID 生成算法，结果是一个 long 型的 ID，
  其核心思想是：使用 41 bit 作为毫秒数，10 bit 作为机器的 
ID，(5 个 ID 是数据中心，5 个 bit 的　ID)，12 bit 作为毫秒内的流水号，(意味着在每个节点每秒可以产生 4096 个 ID)
最后一个是符号位，永远是 0

```html

        snowflake-64 bit
                        41 bit-timeStamp                                12 bit-序列号
        |----------------------------------------------|             |-----------|
        0-00000000 00000000 00000000 00000000 00000000 0-00000000-00-00000000 0000
                                                         |---------|
                                                      10 bit-工作机器 id
```

- seqsvr:高并发下用