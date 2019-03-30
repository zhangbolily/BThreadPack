# BThreadPack #
[![pipeline status](https://gitlab.com/zhangbolily/bthreadpack/badges/master/pipeline.svg)](https://gitlab.com/zhangbolily/bthreadpack/commits/master)
[![Language](https://img.shields.io/badge/language-C%2B%2B11-orange.svg)](https://isocpp.org/)
[![Platforms](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-green.svg)](https://gitlab.com/zhangbolily/bthreadpack)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT/)
[![coverage report](https://gitlab.com/zhangbolily/bthreadpack/badges/master/coverage.svg)](https://gitlab.com/zhangbolily/bthreadpack/commits/master)

[中文版](./README.zh_CN.md)

BThreadPack is an efficient, task driven concurrency
 programming framework.

## Main Features
- Defining a task and execute it are easy.
- Full control of threads at any time, like create and exit.
- Message queue mechanism for inter-thread communication.
- Every task is thread-safe.


## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

If you want to use BThreadPack in your platform, these requirements should be satisfied.

```
C++ Standard: c++11
Build tools: cmake make autoconf automake gcc
```

### Installation

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

## Running the tests

##### Examples

This project contains some examples for a better understanding on how to use these components.

Read [./examples/README.md](./examples/README.md) for more details.

## Versioning

For the versions available, see the [tags on this repository](https://gitlab.com/zhangbolily/bthreadpack/tags).

## Roadmap
Performance improvement in the next version.

## Authors

* **Ball Chang** - *Build the whole project*

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* GitLab
