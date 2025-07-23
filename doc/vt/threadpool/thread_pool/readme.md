vt::thread_pool
===============

- Defined in header `<vt/thread_pool.hpp>`

```c++
class thread_pool;
```

A pool running a user-specified amount of threads that can be reused for work.

Member functions
----------------

|||
----------------------------------- | -----------------------------
[(constructor)](constructor.md#top) | constructs the thread pool
[(destructor)](destructor.md#top)   | destructs the thread pool
[run](run.md#top)                   | runs work on the thread pool
[size](size.md#top)                 | returns the number of threads

Example
-------

```c++
#include <vt/thread_pool.hpp>
#include <algorithm>
#include <iostream>

int main() {
    vt::thread_pool threads{2};

    std::vector<int> x(10000, 1);

    std::future<int> sum0 = threads.run([&] {
        return std::accumulate(x.begin(), x.begin() + 5000, 0);
    });
    std::future<int> sum1 = threads.run([&] {
        return std::accumulate(x.begin() + 5000, x.end(), 0);
    });

    std::cout << (sum0.get() + sum1.get()) << '\n';
}
```

Output:

```
10000
```
