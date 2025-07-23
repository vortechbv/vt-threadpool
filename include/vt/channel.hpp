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

#ifndef VT_CHANNEL_HPP_
#define VT_CHANNEL_HPP_

#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>


namespace vt {

template<typename T, typename Allocator = std::allocator<T>>
class channel {
public:
    channel() = default;
    explicit channel(const Allocator& alloc);

    template<typename... Args>
    void send(Args&&... msg_args);

    T recv();
    std::optional<T> try_recv();
    template<typename Rep, typename Period>
    std::optional<T> try_recv(
        const std::chrono::duration<Rep, Period>& timeout
    );

    void clear();

private:
    std::deque<T, Allocator> _q;
    std::mutex _mutex;
    std::condition_variable _recv_cond;

    std::optional<T> try_pop();
};

} // namespace vt

#include <vt/impl/channel.ipp>

#endif // VT_CHANNEL_HPP_
