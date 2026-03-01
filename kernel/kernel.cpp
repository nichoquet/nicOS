#include "kernel.h"
#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "../drivers/serial.h"   // for early console output
#include "../libc/string.h"
#include "../libc/mem.h"
#include "../libc/boolean.h"
#include "../cpu/n_thread.h"
#include "../libc/List.h"

extern "C"
void kernel_main() {
    // initialize subsystems first
    isr_install();
    irq_install();
    Serial::Initialize();          // serial port for console logs
    // clear_screen();
    MemoryHandler::InitializeInstance();
    test();
    // after test() returns we need to keep yielding so the other threads run
    while (true) {
        n_thread_yield();
    }
}

// simple helper to convert number to string; visible in this file only
static void print_count(const char *thread_name, int count) {
    kprint((string)thread_name);
    kprint(" count=");
    char tmp[12];
    int_to_ascii(count, tmp);
    kprint(tmp);
    kprint("\n");
}

// two threads that will be created by test()
static void thread_fn1() {
    for (int i = 0; i < 20; i++) {
        print_count("[T1]", i);
        // busy‑wait to consume some time so output interleaves
        for (volatile int j = 0; j < 100; j++);
        n_thread_yield();
    }
}

static void thread_fn2() {
    for (int i = 0; i < 20; i++) {
        print_count("[T2]", i);
        for (volatile int j = 0; j < 10; j++);
        n_thread_yield();
    }
}

void test () {
    kprint("-- starting multithreaded test --\n");

    // create two concurrent threads; timer interrupt (already configured in irq_install)
    // will switch between them, so output should alternate.
    n_thread_start(thread_fn1);
    n_thread_start(thread_fn2);

    kprint("threads spawned, returning from test()\n");
    // kernel_main will continue (nothing else) but the scheduler is still active
}

void user_input(char *input) {
    if (strcmp(input, "END") == 0) {
        kprint("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    } else if (strcmp(input, "PAGE") == 0) {
        // uint32_t phys_addr;
        uint32_t page = malloc(1000);
        kprint("Page: ");
        kprintHexAdress(page);
        kprint(", physical address: ");
        kprintHexAdress((int)&page);
        kprint("\n");
    }
    kprint("You said: ");
    kprint(input);
    kprint("\n> ");
}