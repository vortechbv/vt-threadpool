vt::channel
===========

- Defined in header `<vt/channel.hpp>`

```c++
template<typename T, typename Allocator = std::allocator<T>>
class channel;
```

A thread-safe FIFO-queue for the producer/consumer pattern. It supports multiple producers and multiple consumers.

Template parameters
-------------------

|||
------------- | ----------------------------------------------------------------
**T**         | the type of the messages sent over the channel; must be move-constructible
**Allocator** | allocator for acquiring/releasing memory and constructing/destroying elements; `Allocator::value_type` must be the same as `T`

Member functions
----------------

|||
----------------------------------- | ----------------------
[(constructor)](constructor.md#top) | constructs the channel
operator=[deleted]                  | not copy-assignable
[send](send.md#top)                 | sends a message
[recv<br>try_recv](recv.md#top)     | receives a message
[clear](clear.md#top)               | removes messages

Example
-------

```c++
#include <vt/channel.hpp>
#include <iostream>
#include <thread>

int main() {
    vt::channel<int> chan;

    std::thread producer{[&] {
        for (int i = 4; i >= 0; --i) {
            chan.send(i);
        }
    }};

    std::thread consumer{[&] {
        while (true) {
            const int msg = chan.recv();
            if (msg <= 0) break;
            std::cout << msg << '\n';
        }
    }};

    producer.join();
    consumer.join();
}
```

Output:

```
4
3
2
1
```
