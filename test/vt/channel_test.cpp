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

#include <vt/channel.hpp>

#include <catch2/catch_all.hpp>
#include <algorithm>
#include <memory>
#include <thread>


TEST_CASE(
    "A message can be send and received through a vt::channel on a single "
    "thread",
    "[channel]"
) {
    vt::channel<int> chan;

    chan.send(42);
    REQUIRE(chan.recv() == 42);
}


TEST_CASE(
    "A message can be send/received through a vt::channel to/from multiple "
    "threads",
    "[channel]"
) {
    vt::channel<int> chan;
    int recved = 0;

    std::thread sender{[&] { chan.send(42); }};
    std::thread recver{[&] { recved = chan.recv(); }};

    sender.join();
    recver.join();

    REQUIRE(recved == 42);
}


TEST_CASE(
    "A receiver of messages through a vt::channel will block until a sender "
    "has send a message",
    "[channel]"
) {
    vt::channel<int> chan;
    int recved = 0;

    std::thread recver{[&] { recved = chan.recv(); }};
    std::thread sender{[&] {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(10ms);
        chan.send(42);
    }};

    sender.join();
    recver.join();

    REQUIRE(recved == 42);
}


TEST_CASE(
    "A vt::channel can have multiple producers and consumers",
    "[channel]"
) {
    vt::channel<int> chan;
    bool recved[4 * 1024] = {};

    auto produce = [&](int start) {
        for (int i = start; i < start + 1024; ++i) {
            chan.send(i);
        }
    };

    auto consume = [&] {
        for (int i = 0; i < 1024; ++i) {
            recved[chan.recv()] = true;
        }
    };


    std::thread producer0{produce, 0};
    std::thread producer1{produce, 1024};
    std::thread producer2{produce, 2048};
    std::thread producer3{produce, 3072};

    std::thread consumer0{consume};
    std::thread consumer1{consume};
    std::thread consumer2{consume};
    std::thread consumer3{consume};

    producer0.join();
    producer1.join();
    producer2.join();
    producer3.join();

    consumer0.join();
    consumer1.join();
    consumer2.join();
    consumer3.join();

    REQUIRE(std::count(std::begin(recved), std::end(recved), false) == 0);
}


TEST_CASE(
    "A non-copyable message can be send and received through a vt::channel",
    "[channel]"
) {
    vt::channel<std::unique_ptr<int>> chan;

    chan.send(std::make_unique<int>(42));
    std::unique_ptr<int> recved = chan.recv();

    REQUIRE(*recved == 42);
}


TEST_CASE(
    "Multiple messages send through a vt::channel will be received in FIFO "
    "order",
    "[channel]"
) {
    vt::channel<int> chan;

    chan.send(42);
    chan.send(43);

    REQUIRE(chan.recv() == 42);

    chan.send(44);

    REQUIRE(chan.recv() == 43);
    REQUIRE(chan.recv() == 44);
}


TEST_CASE(
    "Try receiving a message through a vt::channel will not block, but return "
    "a std::optional instead",
    "[channel]"
) {
    vt::channel<int> chan;

    REQUIRE(!chan.try_recv());

    chan.send(42);

    REQUIRE(chan.try_recv());
}


TEST_CASE(
    "Try receiving a message through a vt::channel with a timeout will block "
    "until a message is send or the timeout expires",
    "[channel]"
) {
    using namespace std::chrono_literals;

    vt::channel<int> chan;

    const auto t_start = std::chrono::high_resolution_clock::now();
    REQUIRE(!chan.try_recv(10ms));
    const auto t_end = std::chrono::high_resolution_clock::now();
    REQUIRE(t_end - t_start >= 10ms);

    chan.send(42);

    REQUIRE(chan.try_recv(10ms));
}


TEST_CASE(
    "Messages in a vt::channel can be cleared",
    "[channel]"
) {
    vt::channel<int> chan;

    chan.send(42);
    chan.send(43);

    REQUIRE(chan.recv() == 42);

    chan.clear();
    chan.send(44);

    REQUIRE(chan.recv() == 44);
}
