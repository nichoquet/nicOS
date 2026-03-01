#include "serial.h"
#include "../cpu/ports.h"

// traditional I/O port for COM1
#define SERIAL_COM1_BASE                0x3F8

// helper to check whether the transmitter is empty
static inline int is_transmit_empty() {
    return port_byte_in(SERIAL_COM1_BASE + 5) & 0x20;
}

namespace Serial {

void Initialize() {
    // disable all interrupts
    port_byte_out(SERIAL_COM1_BASE + 1, 0x00);
    // enable DLAB (set baud rate divisor)
    port_byte_out(SERIAL_COM1_BASE + 3, 0x80);
    // set divisor to 3 (low byte) 38400 baud
    port_byte_out(SERIAL_COM1_BASE + 0, 0x03);
    //                  (high byte)
    port_byte_out(SERIAL_COM1_BASE + 1, 0x00);
    // 8 bits, no parity, one stop bit
    port_byte_out(SERIAL_COM1_BASE + 3, 0x03);
    // enable FIFO, clear them, with 14-byte threshold
    port_byte_out(SERIAL_COM1_BASE + 2, 0xC7);
    // IRQs enabled, RTS/DSR set
    port_byte_out(SERIAL_COM1_BASE + 4, 0x0B);
}

void Write(char c) {
    while (!is_transmit_empty());
    port_byte_out(SERIAL_COM1_BASE, c);
}

void WriteString(const char *s) {
    for (int i = 0; s[i]; ++i) {
        Write(s[i]);
    }
}

} // namespace Serial
