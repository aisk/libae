# libae

Redis's async event library, extracted as a small reusable C library. This copy is taken from Redis 7.2, the last Redis release still distributed under the BSD license.

## Supported Event Multiplexing Models

* `epoll`
* `kqueue`
* `evport`
* `select`

The backend is selected at compile time according to platform support.

## Build

Build the static library:

```sh
make libae.a
```

Build the static library with CMake:

```sh
cmake -S . -B build
cmake --build build --target ae
```

Build the bundled examples:

```sh
make timer
make echo
```

Build the bundled examples with CMake:

```sh
cmake -S . -B build
cmake --build build
```

Build everything:

```sh
make all
```

Clean generated files:

```sh
make clean
```

Install the library and exported CMake target files:

```sh
cmake -S . -B build
cmake --build build
cmake --install build
```

## Examples

### Timer

The timer example registers multiple time events and prints a message roughly once per second:

```sh
make timer
./timer
```

See [example/timer.c](example/timer.c) for the full source.

### Echo Server

The echo example starts a TCP server on port `8000`:

```sh
make echo
./echo
```

Then connect from another terminal:

```sh
nc 127.0.0.1 8000
```

See [example/echo.c](example/echo.c) for the full source.

## Source Layout

* `src/ae.c` and `src/ae.h`: core event loop implementation and public API
* `src/anet.c` and `src/anet.h`: networking helpers
* `src/monotonic.c` and `src/monotonic.h`: monotonic clock helpers
* `src/ae_epoll.c`, `src/ae_kqueue.c`, `src/ae_evport.c`, `src/ae_select.c`: backend-specific pollers
* `example/`: runnable examples

## Notes

This repository currently builds a static library (`libae.a`) and two example binaries (`timer`, `echo`).

[Original document](http://redis.io/topics/internals-rediseventlib)
