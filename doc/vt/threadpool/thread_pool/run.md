vt::thread_pool::run
====================

```c++
template<typename Func, typename... Args>
std::future<std::invoke_result_t<Func, Args...>> run(Func&& f, Args&&... args) const;
```

Runs the specified function on the first available thread in the pool.

Parameters
----------

|||
-------- | ----------------------------------------------------------
**f**    | the function that will be executed on a thread in the pool
**args** | the arguments that will be passed to this function

Return value
------------

A `std::future` that will contain the result of the function once it has finished.
