typedef void (*n_thread_callback)();
extern void thread_create(n_thread_callback callback);

void n_thread_start(n_thread_callback callback);