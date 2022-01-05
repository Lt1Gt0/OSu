# Table Of Contents

- [Table Of Contents](#table-of-contents)
  - [About](#about)
  - [Branch Structures](#branch-structures)
  - [Running `OSu.img`](#running-osuimg)
    - [Pre-requisites before building and running the OS](#pre-requisites-before-building-and-running-the-os)
      - [Installation](#installation)
  - [TODO](#todo)

## About

__I am not a professional at OS Development by any means.__
__Don't expect this OS to do anything for a long while__

**There is no correlation between this OS and the development team behind the game OSU!**

There is not much to be offered in the OS in its current stages while I learn more OS development and different protocol implementations

- 64-bit OS with simple barebones kernel
- Emulator: QEMU
- Cross compiler: gcc
- Linker: ld
- UEFI bootloader services

## Branch Structures

Master branch of the repository is the now the UEFI branch for the forseen future, the [BIOS](https://github.com/Lt1Gt0/OSu/tree/BIOS) branch as of now will be disregarded and abandoned for the time being, if I feel like it I might go back to working on that branch for fun.

## Running `OSu.img`

### Pre-requisites before building and running the OS

To run the OS.bin file in the release I would reccomend running the `OSu.img` file through qemu

**[Qemu Download Page](https://www.qemu.org/download/)**

- build-essentials
- mtools (4.0.27)
- qemu

#### Installation

``` bash
sudo apt update 
sudo apt-get install build-essentials
sudo apt-get install mtools
```

OSu.img in the current release does not appear for the time being

Assuming QEMU is installed properly and you do have the OSu.img. In the same directory where the .img file is saved, run the command

``` bash
qemu-system-x86_64 -drive file=$(BUILDDIR)/$(OSNAME).img -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none
```

If cloned from the repository itself, to build the bootloader and the kernel, navigate to the file where the repository was cloned in and enter the commands

``` bash
cd gnu-efi/
make bootloader
cd ../kernel
make buildimg
make run
```

## TODO
