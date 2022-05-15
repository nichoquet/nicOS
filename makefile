C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}
CC = /usr/bin/gcc
GDB = /usr/share/gdb
CFLAGS = -g -ffreestanding -Wall -Wextra -fno-exceptions -m32 -fno-PIC

all: clean run

os-image.bin: boot/bootsect.bin kernel.bin
	cat $^ > $@

kernel.bin: boot/kernel_entry.o ${OBJ} cpu/thread_handler.o
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/kernel_entry.o ${OBJ} cpu/thread_handler.o
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ 

run: os-image.bin
	qemu-system-i386 -fda $<

debug: clean os-image.bin kernel.elf
	qemu-system-i386 -s -fda os-image.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o