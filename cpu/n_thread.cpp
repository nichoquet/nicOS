#include "n_thread.h"
#include "../cpu/type.h"
#include "../drivers/screen.h"  // for debug prints
#include "../libc/mem.h"

// very simple round‑robin scheduler with a fixed maximum
static const int MAX_THREADS = 4;

struct Thread {
    uint32_t *esp;
    uint32_t *stack;
    n_thread_callback callback;
    bool active;
};

static Thread threads[MAX_THREADS];
static int thread_count = 0;   // number of *additional* threads (main is index 0)
static int current_thread = -1;

extern "C" void switch_task(uint32_t **old_sp, uint32_t *new_sp);

static void thread_wrapper() {
    int idx = current_thread;
    if (threads[idx].callback) {
        threads[idx].callback();
    }
    // thread finished, mark it inactive and yield forever
    threads[idx].active = false;
    while (true) {
        n_thread_yield();
    }
}

void n_thread_init() {
    for (int i = 0; i < MAX_THREADS; i++) {
        threads[i].esp = nullptr;
        threads[i].stack = nullptr;
        threads[i].callback = nullptr;
        threads[i].active = false;
    }
    thread_count = 0;
    current_thread = -1;
}

// Pre-allocated stacks for threads to avoid malloc issues
static uint32_t thread_stacks[MAX_THREADS][1024];  // 4KB per thread

void n_thread_create(n_thread_callback callback) {
    if (thread_count + 1 >= MAX_THREADS) {
        kprint("n_thread: too many threads\n");
        return;
    }
    int idx = thread_count + 1; // reserve 0 for the "main" thread
    
    // Use pre-allocated stack instead of malloc
    uint32_t *sp = thread_stacks[idx] + 100;  // Start 100 uint32s into the stack
    
    // When popa executes at ESP=sp, it:
    // 1. Pops 8 values: EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX
    // 2. The ESP value (at sp[3]) tells where to read EBX, EDX, ECX, EAX from
    // 3. After all pops complete, ESP is incremented past those 4 values
    // 4. ret pops the return address from the final ESP location
    //
    // Stack layout:
    sp[0] = 0;                      // EDI
    sp[1] = 0;                      // ESI
    sp[2] = 0;                      // EBP
    sp[3] = (uint32_t)(sp + 8);     // ESP -> points to sp[8] for second round of pops
    sp[4] = 0;                      // EBX (first round}
    sp[5] = 0;                      // EDX (first round)
    sp[6] = 0;                      // ECX (first round)
    sp[7] = 0;                      // EAX (first round)
    sp[8] = 0;                      // EBX (second round, after ESP restore)
    sp[9] = 0;                      // EDX (second round)
    sp[10] = 0;                     // ECX (second round)
    sp[11] = 0;                     // EAX (second round)
    sp[12] = (uint32_t)&thread_wrapper;  // return address (for ret after popa)

    threads[idx].esp = sp;
    threads[idx].stack = sp;  // Just store pointer to stack area
    threads[idx].callback = callback;
    threads[idx].active = true;
    thread_count++;
}

void n_thread_yield() {
    if (current_thread < 0) return;
    int old = current_thread;
    int total = thread_count + 1;
    int next = old;
    for (int i = 1; i < total; i++) {
        int cand = (old + i) % total;
        if (threads[cand].active) {
            next = cand;
            break;
        }
    }
    if (next == old) return;
    current_thread = next;
    switch_task(&threads[old].esp, threads[next].esp);
}

void n_thread_start(n_thread_callback callback) {
    if (current_thread == -1) {
        n_thread_init();
        uint32_t esp_val;
        asm volatile("mov %%esp, %0" : "=r"(esp_val));
        threads[0].esp = (uint32_t *)esp_val;
        threads[0].active = true;
        current_thread = 0;
    }
    n_thread_create(callback);
}

/* 32‑bit context switch routine.  Saves the current stack pointer in *old_sp,
   loads the new stack pointer and then performs a `ret` so execution continues
   at the address that was pushed onto the new stack by the thread creator. */
extern "C" void switch_task(uint32_t **old_sp, uint32_t *new_sp) {
    asm volatile(
        "mov 4(%%esp), %%eax\n\t"   /* old_sp pointer */
        "mov 8(%%esp), %%ebx\n\t"   /* new_sp value  */
        "pusha\n\t"
        "mov %%esp, (%%eax)\n\t"   /* save current esp */
        "mov %%ebx, %%esp\n\t"   /* switch to new stack */
        "popa\n\t"
        "ret\n\t"
        :
        :
        : "eax", "ebx"
    );
}
