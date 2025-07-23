vt::channel::send
=================

```c++
template<typename... Args>
void send(Args&&... msg_args);
```

Sends a message over the channel, placing it at the back of the message queue. This will construct the message in-place on the queue.

Parameters
----------

|||
------------ | -----------------------------------------------------------------
**msg_args** | list of arguments that will be forwarded to the constructor of the message object
