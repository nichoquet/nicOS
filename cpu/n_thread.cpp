#include "n_thread.h"
#include "../cpu/type.h"

// const threads = uint32_t[];

void n_thread_start(n_thread_callback callback) {
    callback();
    // thread_create(callback);
}
// void n_thread_create(n_thread_callback callback) {
//     callback();
//     // thread_create(callback);
// }