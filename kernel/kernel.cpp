#include "kernel.h"
#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include <stdint.h>
#include "../libc/boolean.h"
#include "../cpu/n_thread.h"
#include "../libc/List.h"

// void func1 () {
//     kprint_at("Test 1", 0,1);
// }

// void func2 () {
//     kprint_at("Test 2", 0,2);
// }

extern "C"
void kernel_main() {
    isr_install();
    irq_install();
    clear_screen();
    // n_thread_start(&func1);
    // n_thread_start(&func2);
    kprint("Type something, it will go through the kernel\nType END to halt the CPU or PAGE to request a kmalloc()\n> ");
    List<string> lst(3);
    // while(true == true) {
    //     kprint_at("Test", 1,1);
    // }
}
void user_input(char *input) {
    if (strcmp(input, "END") == 0) {
        kprint("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    } else if (strcmp(input, "PAGE") == 0) {
        uint32_t phys_addr;
        uint32_t page = kmalloc(1000, 1, &phys_addr);
        char page_str[16] = "";
        hex_to_ascii(page, page_str);
        char phys_str[16] = "";
        hex_to_ascii(phys_addr, phys_str);
        kprint("Page: ");
        kprint(page_str);
        kprint(", physical address: ");
        kprint(phys_str);
        kprint("\n");
    }
    kprint("You said: ");
    kprint(input);
    kprint("\n> ");
}