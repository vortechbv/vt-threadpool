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

#ifndef VT_THREAD_POOL_HPP_
#define VT_THREAD_POOL_HPP_

#include <vt/channel.hpp>

#include <functional>
#include <future>
#include <thread>
#include <vector>


namespace vt {

class thread_pool {
public:
#ifdef __cpp_lib_move_only_function
    using function_type = std::move_only_function<void()>;
#else
#   pragma message "std::move_only_function unavailable, using std::function"
    using function_type = std::function<void()>;
#endif

    explicit thread_pool(std::size_t thread_count);
    thread_pool(thread_pool&&) = default;

    ~thread_pool();

    thread_pool& operator=(thread_pool&&) = default;

    template<typename Func, typename... Args>
    std::future<std::invoke_result_t<Func, Args...>> run(
        Func&& f,
        Args&&... args
    ) const;

    template<typename Body>
    void parfor(std::size_t n, Body&& body) const;
    template<typename Body>
    void parfor_static(std::size_t n, Body&& body) const;
    template<typename Body>
    void parfor_dynamic(std::size_t n, Body&& body) const;

    std::size_t size() const noexcept;

private:
    mutable channel<function_type> _to_workers;
    std::vector<std::thread> _workers;
};

} // namespace vt

#include <vt/impl/thread_pool.ipp>

#endif // VT_THREAD_POOL_HPP_
