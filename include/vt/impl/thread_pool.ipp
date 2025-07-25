// Copyright (c) 2017-2025 VORtech b.v.
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


#ifdef __cpp_lib_bind_front
#   define VT_THREAD_POOL_BIND std::bind_front
#else
#   pragma message "std::bind_front unavailable, using std::bind"
#   define VT_THREAD_POOL_BIND std::bind
#endif


namespace vt {

template<typename Func, typename... Args>
std::future<std::invoke_result_t<Func, Args...>> thread_pool::run(
    Func&& f,
    Args&&... args
) const {
    using task_type = std::packaged_task<std::invoke_result_t<Func, Args...>()>;

#ifdef __cpp_lib_move_only_function
    task_type task(
        VT_THREAD_POOL_BIND(std::forward<Func>(f), std::forward<Args>(args)...)
    );
    std::future<std::invoke_result_t<Func, Args...>> fut = task.get_future();

    _to_workers.send(std::move(task));

    return fut;
#else
    // If we don't have std::move_only_function, we need to wrap the
    // std::packaged_task in a std::shared_ptr and lambda, since std::function
    // must be copyable.
    auto task = std::make_shared<task_type>(
        VT_THREAD_POOL_BIND(std::forward<Func>(f), std::forward<Args>(args)...)
    );

    _to_workers.send([task] { (*task)(); });

    return task->get_future();
#endif
}


inline std::size_t thread_pool::size() const noexcept {
    return _workers.size();
}

} // namespace vt

#endif // VT_IMPL_THREAD_POOL_IPP_
