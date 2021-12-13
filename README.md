# Table Of Contents

- [Table Of Contents](#table-of-contents)
  - [About](#about)
  - [Cloning the repository](#cloning-the-repository)
  - [Running `OS.bin`](#running-osbin)
  - [TODO](#todo)

## About

**There is no correlation between this OS and the development team behind the game OSU!**

64-bit OS with simple barebones kernel

Emulator: QEMU

Cross compiler: x86_64-elf-gcc

Linker: x86_64-elf-ld

There is not much to be offered in the OS in its current stages while I learn more OS development and different protocol implementations

## Cloning the repository

Development of this project was all made in linux, the GCC Cross compiler used is 'x86_64-elf-gcc' install instruction can be found [here](https://github.com/Absurdponcho/GCCBuildCommands)

If followed on linux, add the environment path to your `~./.bashrc`
  
  in terminal run `sudo nano ~/.bashrc`
  
  at the bottom of the file add `export PATH="$PATH:/usr/local/x86_64elfgcc/bin"`
  
  reboot system with `sudo reboot`

## Running `OS.bin`

To run the OS.bin file in the release I would reccomend running the binary through qemu

**[Qemu Download Page](https://www.qemu.org/download/)**

Assuming Qemu is instlled properly run the command below in terminal in the same directory as `OS.bin`

`qemu-system-x86_64 -drive format=raw,file="OS.bin",index=0,if=floppy -m 128M`

If ran correctly on the release version 1.0.0, there sould be a blue screen in the qemu window and nothing else

If you want to run the most current version of `OS.bin` offered outside of the release version check out [Cloning the repository](#cloning-the-repository). Once your environment is properly setup run the `make` command in the project dirctory which should build the OS and open qemu

## TODO

Kernel

- [ ] VGA Color Support
- [ ] User Input
