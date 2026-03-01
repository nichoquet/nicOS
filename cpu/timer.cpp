#include "timer.h"
#include "isr.h"
#include "ports.h"
#include "../libc/function.h"
#include "n_thread.h"        // scheduler API
#include "../drivers/screen.h"


uint32_t tick = 0;

static void timer_callback(registers_t regs) {
    tick++;
    /* Avoid performing console I/O from the timer interrupt handler.
       Printing (port I/O) from an interrupt can reenter device code
       and trigger host-side iothread locking assertions in QEMU. */
    // preemptive scheduling: force a context switch on every tick
    n_thread_yield();
    UNUSED(regs);
}

void init_timer(uint32_t freq) {
    /* Install the function we just wrote */
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    uint32_t divisor = 1193180 / freq;
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)( (divisor >> 8) & 0xFF);
    /* Send the command */
    port_byte_out(0x43, 0x36); /* Command port */
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}