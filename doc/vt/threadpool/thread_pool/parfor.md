vt::thread_pool::parfor, vt::thread_pool::parfor_static, vt::thread_pool::parfor_dynamic
========================================================================================

```c++
// (1)
template<typename Body>
void parfor(std::size_t n, Body&& body) const;
// (2)
template<typename Body>
void parfor_static(std::size_t n, Body&& body) const;
// (3)
template<typename Body>
void parfor_dynamic(std::size_t n, Body&& body) const;
```

Runs a parallel for loop from 0 to n on the threads in this pool.

1. Uses static scheduling. The static scheduling will be determined by `n` and the number of threads in this pool, so it assumes that none of the threads are currently busy with other work.
2. Same as (1).
3. Uses dynamic scheduling.

Parameters
----------

|||
-------- | ---------------------------------------------------------------------
**n**    | the number of loop iterations
**body** | a function representing the body of the loop, which will be called with the loop counter as argument

Example
-------

```c++
#include <vt/thread_pool.hpp>
#include <iostream>

int main() {
    vt::thread_pool threads{2};

    std::vector<int> x = { 3, 1, 4, 1, 5, 9 };

    threads.parfor(x.size(), [&](std::size_t i) {
        x[i] *= 2;
    });

    for (int xi : x) std::cout << xi << '\n';
}
```

Output:

```
6
2
8
2
10
18
```
