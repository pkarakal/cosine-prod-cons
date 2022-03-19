# cosine calculation: prod-cons implementation  
This project is implemented as a part of a homework exercise for [077] - Real Time Embedded Systems
course of ECE Department, AUTh.

This calculates the cosine of angles using a producer-consumer implementation to find the number of thread
in a multicore system that result in the minimum mean time that a job spends waiting in the queue.

## Getting Started

### Prerequisites
1. CMake
2. Make
3. g++/clang
4. Python (3.6 or higher)
5. Boost
6. pkgconf (Optional)

To install them on variant Linux distributions follow the instructions below

#### Fedora
```shell
$ sudo dnf upgrade --refresh # updates installed packages and repositories metadata
$ sudo dnf install cmake make clang gcc-c++ python pkgconf boost boost-devel
```

#### Ubuntu
```shell
$ sudo apt-get update && sudo apt-get upgrade # updates installed packages and repositories metadata
$ sudo apt-get install cmake make clang g++ python3 pkg-config libboost-all-dev # ubuntu still offers python2 in its repositories
```


[More info incoming as the project progresses]
