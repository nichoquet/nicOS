#include "n_thread.h"
#include "../cpu/type.h"

// const threads = u32[];

void n_thread_start(n_thread_callback callback) {
    callback();
    // thread_create(callback);
}
// void n_thread_create(n_thread_callback callback) {
//     callback();
//     // thread_create(callback);
// }