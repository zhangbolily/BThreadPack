# BThreadPack #
[![pipeline status](https://gitlab.com/zhangbolily/bthreadpack/badges/master/pipeline.svg)](https://gitlab.com/zhangbolily/bthreadpack/commits/master)
[![Language](https://img.shields.io/badge/language-C%2B%2B11-orange.svg)](https://isocpp.org/)
[![Platforms](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-green.svg)](https://gitlab.com/zhangbolily/bthreadpack)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT/)
[![coverage report](https://gitlab.com/zhangbolily/bthreadpack/badges/master/coverage.svg)](https://gitlab.com/zhangbolily/bthreadpack/commits/master)

[中文版](./README.zh_CN.md)

Efficient multi-threading task framework.

## Features
- Satisfied general task usage and specified task usage.
- Full control of threads at any time.
- Message queue mechanism for interthread communication.
- Thread quantity optimization mechanism.
- Every task is thread-safe.


## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

If you want to use BThreadPack in your platform, these requirements need to be satisfied.

```
System: Linux (This project only be tested on this platform)
gcc version >= 4.8 (Need c++11 support)
```

### Source Code Structure
- include: All header files.
- src: All cpp source code files.
- Examples: Some examples tell you how to use this library.

### Installing

BThreadPack doesn't rellay on any third-party moudules(Only STL is required).

##### Step1. Build target

```
make
```

##### Step2. Install into your system

This step need root privilege.

```
make install
```

##### Demonstration build video

![bthreadpack](docs/source/images/linux_build.svg)

Then you can build project in Examples directory to test this library.

## Running the tests

##### Examples

This project contains some examples for a better understanding on how to use these components.

Read [./examples/README.md](./examples/README.md) for more details.

## Versioning

For the versions available, see the [tags on this repository](https://gitlab.com/zhangbolily/bthreadpack/tags).

## Authors

* **Ball Chang** - *Make the whole project*

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* GitLab
