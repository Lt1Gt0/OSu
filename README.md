# OSu
Operating System that plays osu

Nothing much will be displayed currently. Currently in the stages of being a 64 bit OS with nothing much added onto it other than a barebones setup

To run the OS.bin file in the release, have qemu installed and run this command in terminal

`qemu-system-x86_64 -drive format=raw,file="OS.bin",index=0,if=floppy -m 128M`
