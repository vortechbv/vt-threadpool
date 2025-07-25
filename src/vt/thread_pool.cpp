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

#include <vt/thread_pool.hpp>


namespace vt {

namespace {

void worker_main(channel<thread_pool::function_type>& from_pool) {
    while (true) {
        thread_pool::function_type work = from_pool.recv();
        if (!work) break;

        work();
    }
}

} // namespace


thread_pool::thread_pool(std::size_t thread_count) {
    _workers.reserve(thread_count);
    for (std::size_t i = 0; i < thread_count; ++i) {
        _workers.emplace_back(&worker_main, std::ref(_to_workers));
    }
}


thread_pool::~thread_pool() {
    for (std::thread& worker : _workers) {
        (void)worker;
        // An empty function will terminate the worker.
        _to_workers.send(nullptr);
    }

    for (std::thread& worker : _workers) {
        worker.join();
    }
}

} // namespace vt
