# tinyOS

_A tiny operating system for the x86 architecture._

This is a small operating system targeting the x86 architecture (32-bit)
based off of various online tutorials.
The aim is primarily that of learning, and is _far_ from a useable, modern
operating system(!)

## Build status

[![Build Status](https://mjcheetham.visualstudio.com/tinyos/_apis/build/status/mjcheetham.tinyos)](https://mjcheetham.visualstudio.com/tinyos/_build/latest?definitionId=2)

## Features

- [x] Bootable! (using GRUB)
- [x] Set up basic memory segmentation
- [x] Set up paged virtual memory
- [x] Can recieve and handle basic software and hardware (IRQ) interrupts
- [x] Can print to the (VGA text-mode) screen
- [ ] Heaped memory
- [ ] File system
- [ ] User-mode processes and executable loading

## Building

1. Install prerequisites

```shell
# Linux (Ubuntu)
$ apt install make gcc nasm genisoimage bochs-sdl

# macOS
$ brew install cdrtools bochs
```

2. Restore external packages

```shell
$ ./init.sh
```

3. Compile

```shell
$ make all
```

4. Run in the Bochs emulator

```shell
$ make run
```

😊
