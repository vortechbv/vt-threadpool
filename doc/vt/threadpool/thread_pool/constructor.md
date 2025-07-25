vt::thread_pool::thread_pool
============================

```c++
// (1)
explicit thread_pool(std::size_t thread_count);
// (2)
thread_pool(const thread_pool&) = delete;
```

1. Creates the pool and starts the specified amount of threads.
2. Copy constructor is deleted.

Parameters
----------

|||
---------------- | ------------------------------------------
**thread_count** | the number of threads this pool should run
