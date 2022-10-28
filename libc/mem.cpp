#include "mem.h"
#include "boolean.h"
#include "../drivers/screen.h"

void memory_copy(uint8_t *source, uint8_t *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void memory_set(uint8_t *dest, uint8_t val, uint32_t len) {
    uint8_t *temp = (uint8_t *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

/* This should be computed at link time, but a hardcoded
 * value is fine for now. Remember that our kernel starts
 * at 0x1000 as defined on the Makefile */
uint32_t free_mem_addr = 0x10000;

/* Implementation is just a pointer to some free memory which
 * keeps growing */
uint32_t kmalloc(size_t size, int align, uint32_t *phys_addr) {
    // TODO: TO BE Supported
    return MemoryHandler::getNextAvailableAdress();
}

uint32_t malloc(size_t size) {
    uint32_t addr = MemoryHandler::getNextAvailableAdress();
    // kprintHexAdress(addr);
    // kprint("test");
    return addr;
}

void free(void * address) {
    MemoryHandler::freeAdress((uint32_t)address);
}

void *operator new(size_t size)
{
    return (void *)malloc(size);
}
 
void *operator new[](size_t size)
{
    return (void *)malloc(size);
}
 
void operator delete(void *p)
{
    free(p);
}

void operator delete(void *p, unsigned int) {
    free(p);
}
 
void operator delete[](void *p)
{
    free(p);
}

void MemoryHandler::InitializeInstance() {
    uint32_t availablePageAdress = getMemoryTableAdressOfPage(MEMORY_TABLE_AVAILABLE_PAGE_NO);
    for (uint32_t i = 0; i < MEMORY_TABLE_PAGE_SIZE; i++) {
        uint32_t adr = availablePageAdress + i;
        uint32_t * adrPtr = (uint32_t *)adr;
        *adrPtr = false;
    }
    (*(uint32_t *)(availablePageAdress + MEMORY_TABLE_AVAILABLE_PAGE_NO)) = true;
}

uint32_t MemoryHandler::getNextAvailableAdress() {
    uint32_t availablePageAdress = getMemoryTableAdressOfPage(MEMORY_TABLE_AVAILABLE_PAGE_NO);
    uint32_t endOftableAddress = MEMORY_TABLE_NB_OF_PAGES * MEMORY_TABLE_PAGE_SIZE;
    uint32_t address = 0;
    for (int i = MEMORY_TABLE_AVAILABLE_PAGE_NO+1; i < MEMORY_TABLE_PAGE_SIZE; i++) {
        if((*(uint32_t *)(availablePageAdress + i)) == false) {
            (*(uint32_t *)(availablePageAdress + i)) = true;
            address = (endOftableAddress + MEMORY_TABLE_PAGE_SIZE * i);
            break;
        }
    }
    return address;
}

void MemoryHandler::freeAdress(uint32_t address) {
    uint32_t availablePageAdress = getMemoryTableAdressOfPage(MEMORY_TABLE_AVAILABLE_PAGE_NO);
    uint32_t endOftableAddress = MEMORY_TABLE_NB_OF_PAGES * MEMORY_TABLE_PAGE_SIZE;
    uint32_t page = (address - endOftableAddress) / MEMORY_TABLE_PAGE_SIZE;
    (*(uint32_t *)(availablePageAdress + page)) = false;
}

uint32_t MemoryHandler::getMemoryTableAdressOfPage(uint32_t pageNo) {
    return MEMORY_TABLE_ADRESS_START + (pageNo * MEMORY_TABLE_PAGE_SIZE);
}