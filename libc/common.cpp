// common.c -- Defines some global functions.
//             From JamesM's kernel development tutorials.

#include "common.h"

// Write a byte out to the specified port.
void outb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

uint16_t inw(uint16_t port)
{
    uint16_t ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

// Compare two strings. Should return -1 if 
// str1 < str2, 0 if they are equal or 1 otherwise.
// int strcmp(char *str1, char *str2)
// {
//       int i = 0;
//       int failed = 0;
//       while(str1[i] != '\0' && str2[i] != '\0')
//       {
//           if(str1[i] != str2[i])
//           {
//               failed = 1;
//               break;
//           }
//           i++;
//       }
//       // why did the loop exit?
//       if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
//           failed = 1;
  
//       return failed;
// }

// Copy the NULL-terminated string src into dest, and
// return dest.
char *strcpy(char *dest, const char *src)
{
    do
    {
      *dest++ = *src++;
    }
    while (*src != 0);
}

// Concatenate the NULL-terminated string src onto
// the end of dest, and return dest.
char *strcat(char *dest, const char *src)
{
    while (*dest != 0)
    {
        *dest = *dest++;
    }

    do
    {
        *dest++ = *src++;
    }
    while (*src != 0);
    return dest;
}

extern void panic(const char *message, const char *file, uint32_t line)
{
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.

    kprint("PANIC(");
    // kprint(message);
    kprint(") at ");
    // kprint(file);
    kprint(":");
    // monitor_write_dec(line);
    kprint("\n");
    // Halt by going into an infinite loop.
    for(;;);
}

extern void panic_assert(const char *file, uint32_t line, const char *desc)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.

    kprint("ASSERTION-FAILED(");
    // kprint(desc);
    kprint(") at ");
    // kprint(file);
    kprint(":");
    // monitor_write_dec(line);
    kprint("\n");
    // Halt by going into an infinite loop.
    for(;;);
}
