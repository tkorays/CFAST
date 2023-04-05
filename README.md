# CFAST
CFAST, a cross-platform C library for writing client, server and IoT applications.

## Features
This library is a cross-platform library, which support many features.

* Base types definitions.
* Thread, TLS, Socket, DSO(Dynamic Symbol Object).
* Array, List, Queue, HashTable.
* Mutex, Event.
* CLI like python `click`.
* Time and Clock.
* Debug utils.
* Error system.
* File operations.
* Logging.
* Memory management and a session based memory pool.
* Getopt.
* String operations.
* Bytes reader and writer.
* numerics, version utils.

## Platforms
CFAST has been tested on a variety of platforms:

* Mac OSX
* Windows
* Android

## Compile
We use `cmake` to build CFAST library.
```shell
cmake -Bbuild
cmake --build build
```

You can run all tests in linux or mac:
```
cd build
ctest
```

## Author
tkorays: <tkorays@hotmail.com>
