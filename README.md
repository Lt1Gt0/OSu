# Table Of Contents

- [Table Of Contents](#table-of-contents)
  - [About](#about)
  - [Cloning the repository](#cloning-the-repository)
  - [Running `OS.bin`](#running-osbin)
  - [TODO](#todo)

## About

**I am not a professional at OS Development by any means.**
**Don't expect this OS to do anything for a long while**

**There is no correlation between this OS and the development team behind the game OSU!**

There is not much to be offered in the OS in its current stages while I learn more OS development and different protocol implementations

- 64-bit OS with simple barebones kernel
- Emulator: QEMU
- Cross compiler: x86_64-elf-gcc
- Linker: x86_64-elf-ld

Notible Features as of current

- 16 VGA Color Support
- Scancode set for QWERTY Keyboard layout
- Reading text/binary files
- Memory Map display (Show usable memory map locations)

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

If ran correcrtly on the current build of the OS, a bluescreen with keyboard input should open in the qemu window

If you want to run the most current version of `OS.bin` offered outside of the release version check out [Cloning the repository](#cloning-the-repository). Once your environment is properly setup run the `make` command in the project dirctory which should build the OS and open qemu

## TODO

Kernel

- [ ] Terminal
  - [ ] Keyboard
    - [ ] Special Character Scancodes
      - [ ] Backspacing
        - [X] Single backspace
        - [ ] Hold backspace
      - [ ] Arrow keys
      - [ ] lShift/rShift
  - [ ] Mouse Support
  - [ ] Scrolling
- [ ] Memory
  - [ ] Memory mapping
    - [ ] dynamic memory
      - [ ] malloc
      - [ ] free
      - [ ] calloc
      - [ ] memset
      - [ ] realloc
      - [ ] memcpy
      - [ ] alligned_alloc

Code Cleanup

- [ ] Move cpp files to work with headers
- [ ] Clean up includes in kernel
- [ ] Overall simplification
- [ ] 