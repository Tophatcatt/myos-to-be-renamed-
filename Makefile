# Default CFLAGS:
CFLAGS?=-O2 -g
# Add mandatory options to CFLAGS:
CFLAGS:=$(CFLAGS) -Wall -Wextra
buildos:
	export PATH="~/Documents/workspace/cross/bin/"
	i686-elf-as booter/boot.s -o booter/boot.o
	i686-elf-gcc -c kernel/main.c -o kernel/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	i686-elf-gcc -T linker/linker.ld -o myos/myos.bin -ffreestanding -O2 -nostdlib booter/boot.o booter/protected.o kernel/kernel.o dtables/gdtr.o dtables/ihs.o -lgcc
	cp myos/myos.bin myos/isodir/boot/myos.bin
	grub-mkrescue -o myos/myos.iso myos/isodir

buildos_isr:
	export PATH="~/Documents/workspace/cross/bin/"
	i686-elf-as booter/boot.s -o booter/boot.o
	i686-elf-gcc -c kernel/main.c -o kernel/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	i686-elf-gcc -T linker/linker.ld -o myos/myos.bin -ffreestanding -O2 -nostdlib booter/boot.o  kernel/kernel.o dtables/gdtreloader.o dtables/isr.o -lgcc
	cp myos/myos.bin myos/isodir/boot/myos.bin
	grub-mkrescue -o myos/myos.iso myos/isodir

ihs:
	i686-elf-as dtables/ihs.s -o dtables/ihs.o
isr:
	nasm -f elf32 dtables/isr.asm -o dtables/isr.o
runos:
	qemu-system-i386 -cdrom myos/myos.iso
debugos:
	qemu-system-i386 -d int -M smm=off -cdrom myos/myos.iso
gdb:
	gdb myos/myos.bin
	#now enter gdb and enter target remote :1234
