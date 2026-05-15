/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <coroutine>

#include <LibConcurrency/ThreadPool.h>
#include <LibConcurrency/Queue.h>
#include <LibConcurrency/Export.h>

namespace Concurrency {

class CONCURRENCY_API Scheduler {
public:
    Scheduler();

    void submit_main_thread(std::coroutine_handle<> handle);
    void submit_worker_thread(std::coroutine_handle<> handle);

    void update();

    static auto current() -> Scheduler*&;
private:
    ThreadPool m_thread_pool;
    Queue<std::coroutine_handle<>> m_main_thread_queue;
};

}
