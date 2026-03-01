#ifndef SERIAL_H
#define SERIAL_H

#include "../cpu/type.h"        // use project's integer typedefs

namespace Serial {
    // must be called once during early boot before any logging
    void Initialize();

    // write a single character to COM1
    void Write(char c);

    // write a null-terminated string
    void WriteString(const char *s);
}

#endif // SERIAL_H
