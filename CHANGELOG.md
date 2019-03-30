# Changelog
All notable changes to this project will be documented in this file.

The format is based on
 [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to
 [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Added
[@Ball Chang]

- New class BThread for managing thread object better.
- Synchronize mechanism for waiting task finished.
- Auto-destroy task after execution.
- Group task for managing bunch of related tasks.
- Add class BTimer for timing execution time and do timed task.
- Add sphinx documentation support.
- Add task priority feature, high priority task must be scheduled
prior than low priority task.
- Add a new changelog to record the development of this project.
- Many bug fix.

### Changed
[@Ball Chang]

- Using a new style of readme.
- Set more thread properties, such as CPU affinity.
- Follow Google C++ Style.
- Separate the implementation of public members and private members.
- Using cmake to manage and build project.

### Removed
[@Ball Chang]

- Removed the dependency of protobuf in example project.

## [0.1.1-alpha] - 2019-10-22
### Added
[@BallChang]

- Fundamental framework of BThreadPack [@BallChang].
- Message queue mechanism for interthread communication.

[Unreleased]: https://gitlab.com/zhangbolily/bthreadpack/tree/dev
[0.1.1-alpha]:
 https://gitlab.com/zhangbolily/bthreadpack/tree/v0.1.1-alpha

[@Ball Chang]: https://gitlab.com/zhangbolily
