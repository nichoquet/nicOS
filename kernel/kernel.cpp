#include "kernel.h"
#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "../libc/boolean.h"
#include "../cpu/n_thread.h"
#include "../libc/List.h"

extern "C"
void kernel_main() {
    isr_install();
    irq_install();
    clear_screen();
    MemoryHandler::InitializeInstance();
    kprint("Type something, it will go through the kernel\nType END to halt the CPU or PAGE to request a kmalloc()\n> ");
}
void user_input(char *input) {
    if (strcmp(input, "END") == 0) {
        kprint("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    } else if (strcmp(input, "PAGE") == 0) {
        uint32_t phys_addr;
        uint32_t page = kmalloc(1000, 1, &phys_addr);
        kprint("Page: ");
        kprintHexAdress(page);
        kprint(", physical address: ");
        kprintHexAdress(phys_addr);
        kprint("\n");
    }
    kprint("You said: ");
    kprint(input);
    kprint("\n> ");
}