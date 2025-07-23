vt::channel::recv, vt::channel::try_recv
========================================

```c++
// (1)
T recv();
// (2)
std::optional<T> try_recv();
// (3)
template<typename Rep, typename Period>
std::optional<T> try_recv(const std::chrono::duration<Rep, Period>& timeout);
```

Receives the message at the front of the channel's message queue, removing that message from the queue.

1. If no messages are present in the queue, this call will block until a message is available.
2. If no messages are present in the queue, this call will return directly.
3. If no messages are present in the queue, this call will block until a message is available or the specified timeout duration has expired.

Parameters
----------

|||
----------- | -----------------------------------------------------------------
**timeout** | the maximum duration this call will block when no message is available

Return value
------------

1. The message at the front of the queue.
2. The message at the front of the queue, wrapped in a `std::optional`, or an empty `std::optional` if no messages are present in the queue.
3. The message at the front of the queue, wrapped in a `std::optional`, or an empty `std::optional` if no messages are present in the queue when the timeout expires.
