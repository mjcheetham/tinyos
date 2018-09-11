# tinyOS

_A tiny operating system for the x86 architecture._

This is a small operating system targeting the x86 architecture (32-bit)
based off of various online tutorials.
The aim is primarily that of learning, and is _far_ from a useable, modern
operating system(!)

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

### Prerequisites

Install using your favourite package manager...

- make
- gcc
- nasm
- bochs-sdl
- genisoimage

When you first build the project, a copy of the GRUB `stage2_eltorito`
bootloader binary is downloaded which allows the kernel to boot from
the CDROM ISO.

### Compile and run

The included makefile can build the ISO layout (and image) as well as start
the Bochs x86 emulator.

To build the iso, execute:

```shell
$ make
```

To build and run in the Bochs emulator, execute:

```shell
$ make run
```

😊
