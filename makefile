# C_SOURCES = $(wildcard kernel/*.cpp)
# HEADERS = $(wildcard kernel/*.h)
C_SOURCES = $(wildcard kernel/*.cpp drivers/*.cpp cpu/*.cpp libc/*.cpp)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)
BUILD_DIR = build
OBJ = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(C_SOURCES)) $(BUILD_DIR)/cpu/interrupt.o
CC = /usr/bin/gcc
GDB = /usr/share/gdb
CFLAGS = -ffreestanding -Wall -Wextra -fno-exceptions -m32 -fno-PIC -w
CPPFLAGS = -g -ffreestanding -O2 -fno-rtti -Wall -Wextra -fno-exceptions -m32 -fno-PIC -Wno-write-strings -libsupc++

IMAGE = $(BUILD_DIR)/os-image.bin
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
KERNEL_ELF = $(BUILD_DIR)/kernel.elf

all: clean run

$(KERNEL_BIN): $(BUILD_DIR)/boot/kernel_entry.o $(OBJ)
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

$(KERNEL_ELF): $(BUILD_DIR)/boot/kernel_entry.o $(OBJ)
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ 

$(IMAGE): $(BUILD_DIR)/boot/bootsect.bin $(KERNEL_BIN)
	cat $^ > $@

QEMU_EXTRA ?=
QEMU_FLAGS = -m 512 -smp 4 -serial stdio $(QEMU_EXTRA)

# The -serial stdio argument causes QEMU to redirect COM1 to the host
# terminal.  Any characters sent to the serial port (0x3F8) by the kernel
# will appear in the QEMU console, which we use for logging.

run: $(IMAGE)
	qemu-system-i386 -fda $< $(QEMU_FLAGS)

debug: clean $(IMAGE) $(KERNEL_ELF)
	qemu-system-i386 -s -fda $(IMAGE) &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file $(KERNEL_ELF)"

$(BUILD_DIR)/%.o: %.cpp ${HEADERS}
	mkdir -p $(dir $@)
	${CC} ${CPPFLAGS} -ffreestanding -c $< -o $@

$(BUILD_DIR)/%.o: %.asm
	mkdir -p $(dir $@)
	nasm $< -f elf -o $@

$(BUILD_DIR)/%.bin: %.asm
	mkdir -p $(dir $@)
	nasm $< -f bin -o $@

clean:
	rm -rf $(BUILD_DIR)