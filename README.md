# MCR

[![Passing Status](https://github.com/Shiny-Man/img.org/blob/master/passing.svg)](https://github.com/Shiny-Man/mcr)
[![Bulid Status](https://github.com/Shiny-Man/img.org/blob/master/build.svg)](https://github.com/Shiny-Man/mcr)

## what is MCR

简单来说 MCR 是一个基于 UDP socket 的多人聊天项目

- 项目总体基于生产者消费之模型
- 借助第三方库 ncurses 图形库实现聊天界面

## 项目功能

- 支持自己与自己尬聊 (PS: 基于 echoserver)
- 支持一对一消息模型
- 支持一对多消息模型
- 支持剔除用户功能 (PS: 当且仅当客户端主动退出，服务端收到 "quit" 才进行剔除)

## 项目不足

- UDP 的可靠性问题
- 服务端发送消息时发现客户端已经离线 or 退出，导致信息丢失问题

## 项目优化

- UDP 的可靠性问题
  - 实现 sql/ack 机制来解决 UDP 的可靠性问题
  - 具体实现为：当客户端发送消息给对端，如果对端收到之后，并且显示的返回改客户端一个 ack，发送方接收到 ack 就确认传输消息成功，如果没有收到 ack，sleep(3) 再发送原来的信息给对端

- 服务端发送消息时发现客户端已经离线 or 退出，导致信息丢失问题
  - 当服务端发现客户端离线 or 中断退出时，将发送的信息回滚到数据池当中，或者实现一个发送/接收缓冲池，把即将丢失的信息回滚到改缓冲池当中

## 所得经验
