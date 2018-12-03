# BThreadPack #
[![pipeline status](https://gitlab.com/zhangbolily/bthreadpack/badges/master/pipeline.svg)](https://gitlab.com/zhangbolily/bthreadpack/commits/master)
[![coverage report](https://gitlab.com/zhangbolily/bthreadpack/badges/master/coverage.svg)](https://gitlab.com/zhangbolily/bthreadpack/commits/master)

A handful thread tool kit.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

If you want to use BThreadPack in your platform, these requirements need to be satisfied.

```
System: Linux (This project only be tested on this platform)
gcc version > 4.8 (Need c++11 support)
```

### Source Code Structure
- include: All header files.
- src: All cpp source code files.
- Examples: Some examples tell you how to use this library.

### Installing

BThreadPack doesn't rellay on any third-party moudules.

##### Step1. Build target

```
make
```

##### Step2. Install into your system

This step need root privilege.

```
make install
```

Then you can build project in Examples directory to test this library.

## Running the tests

##### MultiThreadsHelloWorld

This project is in Examples directory. Enter Examples/MultiThreadsHelloWorld directory and execute these commands.

```
make
bin/MultiThreadsHelloWorld
```

Then terminal will output many Hello Worlds!

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags).

## Authors

* **Ball Chang** - *Make the whole project*

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* GitLab
