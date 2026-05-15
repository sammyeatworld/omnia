/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <cassert>
#include <coroutine>

#include <LibConcurrency/Scheduler.h>

namespace Concurrency {

struct SwitchToMainThread {
    auto await_ready() const noexcept -> bool
    {
        return false;
    }

    void await_suspend(std::coroutine_handle<> handle) const noexcept
    {
        assert(Scheduler::current() != nullptr && "Awaitable used without a scheduler context.");
        Scheduler::current()->submit_main_thread(handle);
    }

    void await_resume() const noexcept { }
};

struct SwitchToWorkerThread {
    auto await_ready() const noexcept -> bool
    {
        return false;
    }

    void await_suspend(std::coroutine_handle<> handle) const noexcept
    {
        assert(Scheduler::current() != nullptr && "Awaitable used without a scheduler context.");
        Scheduler::current()->submit_worker_thread(handle);
    }

    void await_resume() const noexcept { }
};

}
