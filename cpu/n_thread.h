#ifdef __cplusplus
extern "C" {
#endif

typedef void (*n_thread_callback)();

// start the threading system and/or add a new thread
void n_thread_start(n_thread_callback callback);

// voluntarily yield the CPU to another runnable thread
void n_thread_yield();

#ifdef __cplusplus
}
#endif