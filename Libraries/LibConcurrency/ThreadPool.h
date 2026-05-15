/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <functional>
#include <queue>
#include <thread>
#include <condition_variable>

#include <Common/Noncopyable.h>
#include <Common/Types.h>

namespace Concurrency {

using Job = std::function<void()>;

class ThreadPool {
    OA_MAKE_NONCOPYABLE(ThreadPool);
    OA_MAKE_NONMOVABLE(ThreadPool);

public:
    ThreadPool(u32 thread_count = std::thread::hardware_concurrency());
    ThreadPool(std::function<void()> thread_init_callback, u32 thread_count = std::thread::hardware_concurrency());
    ~ThreadPool();

    void submit(Job job);
private:
    void worker(std::stop_token const& stop_token);
private:
    std::vector<std::jthread> m_workers;
    std::mutex m_worker_mutex;
    std::queue<Job> m_job_queue;
    std::condition_variable m_thread_pool_cv;
};

}
