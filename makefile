C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o}
CC = /usr/bin/gcc
GDB = /usr/share/gdb
# -g: Use debugging symbols in gcc
CFLAGS = -g -m32 -fno-PIC

all: clean run





# kernel_entry.o: boot/kernel_entry.asm
# 	nasm $< -f elf -o build/$@

# kernel.o: kernel.c
# 	gcc -m32 -fno-pie -c $< -o $@

# kernel.dis: kernel.bin
# 	ndisasm -b 32 $< > $@

# bootsect.bin: bootsect.asm
# 	nasm $< -f bin -o $@

os-image.bin: boot/bootsect.bin kernel.bin
	cat $^ > $@

kernel.bin: boot/kernel_entry.o ${OBJ}
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/kernel_entry.o ${OBJ}
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ 

run: os-image.bin
	qemu-system-i386 -fda $<

debug: os-image.bin kernel.elf
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
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o