#include "n_thread.h"

void n_thread_start(n_thread_callback callback) {
    thread_create(callback);
}