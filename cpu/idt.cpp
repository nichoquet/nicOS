#include "idt.h"
#include "type.h"

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt_gate(int n, idt_handler_function handler) {
    u32 address = *((u32*)(&handler));
    idt[n].low_offset = low_16(address);
    idt[n].sel = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; 
    idt[n].high_offset = high_16(address);
}

void set_idt() {
    idt_reg.base = *((u32*)(&idt));
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}