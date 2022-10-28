#ifndef MEM_H
#define MEM_H

#include <stdint.h>
#include <stddef.h>

#define MEMORY_TABLE_ADRESS_START 0x10000 // 65 536
#define MEMORY_TABLE_PAGE_SIZE 0x1000 // 4K
#define MEMORY_TABLE_BLOCK_SIZE 0x1000 // 4K
#define MEMORY_TABLE_AVAILABLE_PAGE_NO 2
#define MEMORY_TABLE_NB_OF_PAGES 2

void memory_copy(uint8_t *source, uint8_t *dest, int nbytes);
void memory_set(uint8_t *dest, uint8_t val, uint32_t len);

uint32_t kmalloc(size_t size, int align, uint32_t *phys_addr);
uint32_t malloc(size_t size);
void free(void * address);

void *operator new(size_t size);
 
void *operator new[](size_t size);
 
void operator delete(void *p);
void operator delete(void *p, unsigned int);
 
void operator delete[](void *p);


class MemoryHandler {
    private:
        static uint32_t getMemoryTableAdressOfPage(uint32_t pageNo);
    public:
        static uint32_t getNextAvailableAdress();
        static void freeAdress(uint32_t address);
        static void InitializeInstance();
};

#endif