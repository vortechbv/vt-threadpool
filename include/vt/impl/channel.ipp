// Copyright (c) 2017-2018 VORtech b.v.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef VT_IMPL_CHANNEL_IPP_
#define VT_IMPL_CHANNEL_IPP_

#include <functional>


namespace vt {

template<typename T, typename Allocator>
channel<T, Allocator>::channel(const Allocator& alloc)
    : _q{alloc} {
}


template<typename T, typename Allocator>
template<typename... Args>
void channel<T, Allocator>::send(Args&&... msg_args) {
    std::unique_lock<std::mutex> lock{_mutex};

    _q.emplace_back(std::forward<Args>(msg_args)...);

    // There appears to be a bug in the pthreads implementation used by MinGW
    // where a race-condition occurs in notify_one() that can deadlock when
    // there are multiple receivers.
    // Not unlocking before notifying reduces performance, but for MinGW we
    // don't really have a viable alternative at the moment.
#ifndef __MINGW32__
    lock.unlock();
#endif
    _recv_cond.notify_one();
}


template<typename T, typename Allocator>
T channel<T, Allocator>::recv() {
    std::unique_lock<std::mutex> lock{_mutex};

    _recv_cond.wait(lock, [this] { return !_q.empty(); });

    T msg = std::move(_q.front());
    _q.pop_front();

    return msg;
}


template<typename T, typename Allocator>
std::optional<T> channel<T, Allocator>::try_recv() {
    std::unique_lock<std::mutex> lock{_mutex};

    return this->try_pop();
}


template<typename T, typename Allocator>
template<typename Rep, typename Period>
std::optional<T> channel<T, Allocator>::try_recv(
    const std::chrono::duration<Rep, Period>& timeout
) {
    std::unique_lock<std::mutex> lock{_mutex};

    _recv_cond.wait_for(lock, timeout, [this] { return !_q.empty(); });

    return this->try_pop();
}


template<typename T, typename Allocator>
void channel<T, Allocator>::clear() {
    std::unique_lock<std::mutex> lock{_mutex};

    _q.clear();
}


template<typename T, typename Allocator>
std::optional<T> channel<T, Allocator>::try_pop() {
    if (_q.empty()) return std::nullopt;

    std::optional<T> msg = std::move(_q.front());
    _q.pop_front();

    return msg;
}

} // namespace vt

#endif // VT_IMPL_CHANNEL_IPP_
