/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <atomic>
#include <mutex>
#include <optional>
#include <queue>

#include <Common/Types.h>

namespace Concurrency {

template<typename T>
class Queue {
public:
    auto pop() -> std::optional<T>
    {
        std::unique_lock lock(m_mutex);
        if (m_queue.empty()) {
            return std::nullopt;
        }
        auto value = std::move(m_queue.front());
        m_queue.pop();
        return value;
    }

    void push(T&& value)
    {
        std::unique_lock lock(m_mutex);
        m_queue.push(std::move(value));
    }

    void push(const T& value)
    {
        std::unique_lock lock(m_mutex);
        m_queue.push(value);
    }

    auto empty() -> bool
    {
        std::unique_lock lock(m_mutex);
        return m_queue.empty();
    }

    auto collect() -> std::queue<T>
    {
        std::queue<T> collected_queue;
        {
            std::unique_lock lock(m_mutex);
            std::ranges::swap(m_queue, collected_queue);
        }
        return collected_queue;
    }
private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
};

}
