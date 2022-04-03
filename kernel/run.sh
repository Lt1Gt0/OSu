cd ../gnu-efi
make
make bootloader
cd ../kernel
make kernel
make buildimg
make run