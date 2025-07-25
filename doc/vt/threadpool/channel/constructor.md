vt::channel::channel
====================

```c++
// (1)
channel() = default;
// (2)
explicit channel(const Allocator& alloc);
// (3)
channel(const channel&) = delete;
```

Constructs an empty channel, optionally using a user supplied allocator `alloc`.

1. Default constructor. Constructs an empty channel, with a default-constructed allocator.
2. Constructs an empty channel with the specified allocator.
3. Copy constructor is deleted.

Parameters
----------

|||
--------- | -----------------------------------------------------------
**alloc** | allocator to use for all memory allocations of this channel
