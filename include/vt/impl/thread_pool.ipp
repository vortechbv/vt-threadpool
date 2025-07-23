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

#ifndef VT_IMPL_THREAD_POOL_IPP_
#define VT_IMPL_THREAD_POOL_IPP_

#include <memory>
#include <vector>


namespace vt {

template<typename Func, typename... Args>
std::future<std::invoke_result_t<Func, Args...>> thread_pool::run(
    Func&& f,
    Args&&... args
) const {
    // TODO: For the case where there are no arguments, we can skip the call to
    // std::bind(). Would that improve performance?

    using task_type = std::packaged_task<std::result_of_t<Func(Args...)>()>;

    // Using a shared pointer, since std::function apparently needs to make a
    // copy at some point.
    auto task = std::make_shared<task_type>(
        std::bind(std::forward<Func>(f), std::forward<Args>(args)...));

    _to_workers.send([task] { (*task)(); });

    return task->get_future();
}


inline std::size_t thread_pool::size() const noexcept {
    return _workers.size();
}

} // namespace vt

#endif // VT_IMPL_THREAD_POOL_IPP_
