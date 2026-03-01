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

/* Implementation is just a pointer to some free memory which
 * keeps growing */
uint32_t kmalloc(uint32_t size, int align, uint32_t *phys_addr) {
    kprint("kmalloc(size=");
    kprintHexAdress(size);
    kprint(", align=");
    kprintHexAdress(align);
    kprint(")\n");
    uint32_t addr = MemoryHandler::getNextAvailableAdressAndUseIt();
    kprint(" -> returning ");
    kprintHexAdress(addr);
    kprint("\n");
    return addr;
}

uint32_t malloc(uint32_t size) {
    kprint("malloc(size=");
    kprintHexAdress(size);
    kprint(")\n");
    uint32_t addr = MemoryHandler::getNextAvailableAdressAndUseIt();
    kprint(" -> ");
    kprintHexAdress(addr);
    kprint("\n");
    return addr;
}

void free(void * address) {
    kprint("free(addr=");
    kprintHexAdress((uint32_t)address);
    kprint(")\n");
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

/* helper macros for the allocator; we keep the old function names for
   backwards compatibility but the underlying implementation is much
   simpler now. */
static inline uint32_t memory_table_base() {
    return MEMORY_TABLE_ADRESS_START;
}

static inline uint32_t memory_data_base() {
    /* first page (PAGE_SIZE bytes) is reserved for the table itself, so data
       starts one page later */
    return MEMORY_TABLE_ADRESS_START + MEMORY_TABLE_PAGE_SIZE;
}

static uint32_t *memory_table() {
    return (uint32_t *)memory_table_base();
}

void MemoryHandler::InitializeInstance() {
    kprint("Initialize memory table\n");
    uint32_t *table = memory_table();
    for (uint32_t i = 0; i < MEMORY_TABLE_NB_OF_PAGES; i++) {
        table[i] = false;
        kprint(" table["); kprintHexAdress(i); kprint("]=0\n");
    }
}

uint32_t MemoryHandler::getNextAvailableAdress() {
    uint32_t *table = memory_table();
    uint32_t base = memory_data_base();
    for (uint32_t i = 0; i < MEMORY_TABLE_NB_OF_PAGES; i++) {
        if (!table[i]) {
            return base + (i * MEMORY_TABLE_PAGE_SIZE);
        }
    }
    return 0;
}

uint32_t MemoryHandler::getNextAvailableAdressAndUseIt() {
    uint32_t *table = memory_table();
    uint32_t base = memory_data_base();
    kprint("getNextAvailableAdressAndUseIt: table@ ");
    kprintHexAdress((uint32_t)table);
    kprint(" base="); kprintHexAdress(base); kprint("\n");
    for (uint32_t i = 0; i < MEMORY_TABLE_NB_OF_PAGES; i++) {
        kprint(" check page "); kprintHexAdress(i);
        kprint(" value="); kprintHexAdress(table[i]); kprint("\n");
        if (!table[i]) {
            table[i] = true;
            uint32_t addr = base + (i * MEMORY_TABLE_PAGE_SIZE);
            kprint(" -> allocating page "); kprintHexAdress(i);
            kprint(" at "); kprintHexAdress(addr); kprint("\n");
            return addr;
        }
    }
    kprint(" -> no free page\n");
    return 0;
}

void MemoryHandler::freeAdress(uint32_t address) {
    kprint("freeAdress( "); kprintHexAdress(address); kprint(" )\n");
    uint32_t base = memory_data_base();
    if (address < base) {
        kprint(" address below data base\n");
        return; // invalid address
    }
    uint32_t offset = address - base;
    if (offset % MEMORY_TABLE_PAGE_SIZE != 0) {
        kprint(" address not page aligned\n");
        return; // must be page-aligned
    }
    uint32_t page = offset / MEMORY_TABLE_PAGE_SIZE;
    if (page < MEMORY_TABLE_NB_OF_PAGES) {
        uint32_t *table = memory_table();
        kprint(" freeing page "); kprintHexAdress(page);
        kprint("\n");
        table[page] = false;
    } else {
        kprint(" page index out of range\n");
    }
}

uint32_t MemoryHandler::getMemoryTableAdressOfPage(uint32_t pageNo) {
    /* this function is now only used internally for debugging and is
       essentially a thin wrapper over the original formula. */
    return MEMORY_TABLE_ADRESS_START + (pageNo * MEMORY_TABLE_PAGE_SIZE);
}