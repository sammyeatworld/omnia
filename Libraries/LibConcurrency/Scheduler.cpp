/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibConcurrency/Scheduler.h>

namespace Concurrency {

Scheduler::Scheduler()
    : m_thread_pool([this] {
        Scheduler::current() = this;
    })
{
    current() = this;
}

void Scheduler::submit_main_thread(std::coroutine_handle<> handle)
{
    m_main_thread_queue.push(handle);
}

void Scheduler::submit_worker_thread(std::coroutine_handle<> handle)
{
    m_thread_pool.submit([handle] {
        handle.resume();
    });
}

void Scheduler::update()
{
    auto main_thread_queue = m_main_thread_queue.collect();
    while (!main_thread_queue.empty()) {
        auto handle = main_thread_queue.front();
        main_thread_queue.pop();
        handle.resume();
    }
}

auto Scheduler::current() -> Scheduler*&
{
    thread_local Scheduler* instance = nullptr;
    return instance;
}

}
