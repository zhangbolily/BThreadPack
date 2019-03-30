# BThreadPack #
[![Build Status](https://travis-ci.com/zhangbolily/BThreadPack.svg?branch=dev)](https://travis-ci.com/zhangbolily/BThreadPack)
[![Language](https://img.shields.io/badge/language-C%2B%2B11-orange.svg)](https://isocpp.org/)
[![Platforms](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-green.svg)](https://gitlab.com/zhangbolily/bthreadpack)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT/)
[![coverage report](https://gitlab.com/zhangbolily/bthreadpack/badges/master/coverage.svg)](https://gitlab.com/zhangbolily/bthreadpack/commits/master)

[English](./README.md)

BThreadPack 是一个高效、任务驱动的并发编程框架。

## 主要特性
- 轻松地定义和执行任务
- 随时可以控制线程池的运转，例如创建和退出线程
- 用于线程间通讯的消息队列机制
- 每个任务都是线程安全的

## 如何开始

通过以下步骤，你将获得本项目的源代码用于开发或者测试。

### 系统要求

若你想在你的平台上使用 BThreadPack，必须满足以下条件。
```
C++ 标准: c++11
构建工具: cmake make autoconf automake gcc
```

### 安装

```
$ git clone https://gitlab.com/zhangbolily/bthreadpack.git BThreadPack
$ cd BThreadPack
$ mkdir build
$ cd build
$ cmake .. 
$ make
$ make install
```

##### Demonstration build video (click to veiw)

[![Build](https://asciinema.org/a/4Sh30mIfX3uw6hboIJWFkXqbf.svg)](https://asciinema.org/a/4Sh30mIfX3uw6hboIJWFkXqbf)

## 运行示例程序

##### Examples

此文件夹下包含了一些用于帮助你理解如何使用本工具包的示例项目。

阅读 [./examples/README.md](./examples/README.md) 获取更多信息。

## 版本信息

获取可用版本，请参阅 [tags on this repository](https://gitlab.com/zhangbolily/bthreadpack/tags)。

## 路线图
在下个版本进行性能提升。

## 作者

* **Ball Chang** - *构建了整个项目*

## 许可证

本项目使用了 MIT 许可 - 阅读 [LICENSE.md](LICENSE.md) 获取更多信息。

## 致谢

* GitLab
