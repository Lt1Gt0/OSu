# OSu

__I am not a professional at OS Development by any means.__
__Don't expect this OS to do anything for a long while__

## Table Of Contents

- [OSu](#osu)
  - [Table Of Contents](#table-of-contents)
  - [Disclaimer](#disclaimer)
  - [About](#about)
  - [Notable Features](#notable-features)
  - [Branch Structures](#branch-structures)
  - [Running `OSu.img`](#running-osuimg)
    - [Pre-requisites before building and running the OS](#pre-requisites-before-building-and-running-the-os)
      - [Installation](#installation)
  - [TODO](#todo)

## Disclaimer

  __There is no correlation between this OS and the development team behind the game OSU!__

  As a restatment from the beginning I am not a professional at OS Development by any means. Don't expect this OS to do anything for a long while

  This was started as a hobby project to play around with CPU architecture and learning computers at a fundamental level.

  Also I just thought that `OSU` and `OSu` would be kinda funny because of the name to be honest

## About

There is not much to be offered in the OS in its current stages while I learn more OS development and different protocol implementations

- 64-bit OS with simple barebones kernel
- Emulator: QEMU
- Cross compiler: gcc
- Linker: ld
- UEFI bootloader services

To my kowledge this should work outside of QEMU as in real hardware, I have yet to try it out, but later version I will experiment further with real hardware

## Notable Features

- PS2 Keyboard support
- PS2 Mouse support (kind of)
- Simple frame buffer
- UEFI bootloader
- PCI
- Simple heap
- PIT

## Branch Structures

Master branch of the repository is the now the UEFI branch for the forseen future, the [BIOS](https://github.com/Lt1Gt0/OSu/tree/BIOS) branch as of now will be disregarded and abandoned for the time being, if I feel like it I might go back to working on that branch for fun.

## Running `OSu.img`

### Pre-requisites before building and running the OS

To run the OS.bin file in the release I would reccomend running the `OSu.img` file through qemu

__[Qemu Download Page](https://www.qemu.org/download/)__

- build-essentials
- mtools (4.0.27)
- qemu
- g++
- nasm

#### Installation

``` bash
sudo apt update 
sudo apt-get install build-essentials
sudo apt-get install qemu
sudo apt-get install g++
sudo apt-get install nasm
```

The mtools installation should work fine on ubunutu 20.04 LTS
If it doesn't work then install mtools as so

```bash
wget -S ftp.gnu.org/gnu/mtools/mtools-4.0.27.tar.gz
tar xf mtools-4.0.27.tar.gz
cd mtools-4.0.27
./configure
sudo make install
```

If for some reason qemu doesn't work, go to the [Qemu Download Page](https://www.qemu.org/download/) and follow the instruction for installtion there

Assuming QEMU is installed properly and you do have the OSu.img. In the same directory where the .img file is saved, run the command

``` bash
qemu-system-x86_64 -machine q35 -drive file=$(BUILDDIR)/$(OSNAME).img -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none
```

To build OSu from source navigate into the kernel directory and run

```bash
./run.sh
```

to clean the source run

```bash
./clean.sh
```

## TODO

- USB
  - [ ] 2.0
  - [ ] 3.0
- networking capabilities
  - Unknown currently
- [ ] Test OS on real hardware
- [ ] AHCI
