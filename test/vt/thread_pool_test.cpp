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

#include <vt/thread_pool.hpp>

#include <catch2/catch_all.hpp>
#include <stdexcept>


TEST_CASE(
    "Work can be run on a vt::thread_pool asynchronously",
    "[thread_pool]"
) {
    vt::thread_pool threads{2};

    auto work = [](int x) { return 5 * x - 3; };

    std::future<int> y0 = threads.run(work, 4);
    std::future<int> y1 = threads.run(work, 2);
    std::future<int> y2 = threads.run(work, 3);

    REQUIRE(y0.get() == work(4));
    REQUIRE(y1.get() == work(2));
    REQUIRE(y2.get() == work(3));
}


TEST_CASE(
    "Work run on a vt::thread_pool propagates exceptions back to the calling "
    "thread",
    "[thread_pool]"
) {
    vt::thread_pool threads{1};

    std::future<void> fut = threads.run([] { throw std::runtime_error{""}; });

    REQUIRE_THROWS_AS(fut.get(), std::runtime_error);
}


TEST_CASE(
    "The number of threads in a vt::thread_pool can be queried",
    "[thread_pool]"
) {
    vt::thread_pool threads{3};

    REQUIRE(threads.size() == 3);
}
