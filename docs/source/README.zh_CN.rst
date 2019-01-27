.. -*- coding: utf-8 -*-

.. _rst:

==============
BThreadPack
==============

.. image:: https://gitlab.com/zhangbolily/bthreadpack/badges/master/pipeline.svg
    :alt: GitLab
    :target: https://gitlab.com/zhangbolily/bthreadpack/commits/master
    
.. image:: https://gitlab.com/zhangbolily/bthreadpack/badges/master/coverage.svg
    :alt: GitLab
    :target: https://gitlab.com/zhangbolily/bthreadpack/commits/master
    
.. image:: https://readthedocs.org/projects/bthreadpack/badge/?version=latest
    :target: http://bthreadpack.readthedocs.io/
    :alt: Documentation Status

好用的线程池工具包。

==============
特性
==============
- 能够满足通用任务和特定任务两种线程池模型
- 随时可以控制线程池的运转
- 用于线程间通讯的消息队列机制
- 线程数量优化机制
- 每个任务都是线程安全的

==============
如何开始
==============

通过以下步骤，你将获得本项目的源代码用于开发或者测试。

----------
系统要求
----------

若你想在你的平台上使用 BThreadPack，必须满足以下条件。
```
系统: Linux (此项目只在Linux系统上进行过测试)
gcc 版本 > 4.8 (需要 c++11 支持)
```

----------
源代码结构
----------
- include: 所有的头文件
- src: 所有的源代码
- Examples: 一些示例帮助你理解如何使用这个工具包

----------
安装
----------

BThreadPack 不依赖任何第三方模块（只需要STL）。

步骤1. 构建目标文件
--------------------

```
make
```

步骤2. 安装到系统
--------------------

此步骤需要root权限。

```
make install
```

安装成功后，就可以构建 Examples 目录下的示例用于测试。

运行示例程序

--------------------
Examples
--------------------

此文件夹下包含了一些用于帮助你理解如何使用本工具包的示例项目。

阅读 [./examples/README.md](./examples/README.md) 获取更多信息。

----------
版本信息
----------

获取可用版本，请参阅 [tags on this repository](https://gitlab.com/zhangbolily/bthreadpack/tags)。

----------
作者
----------

* **Ball Chang** - *构建了整个项目*

----------
许可证
----------

本项目使用了 MIT 许可 - 阅读 [LICENSE.md](LICENSE.md) 获取更多信息。

----------
致谢
----------

* GitLab
