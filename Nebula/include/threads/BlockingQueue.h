//
// Created by michal-swiatek on 14.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#include <queue>
#include <mutex>
#include <optional>
#include <semaphore>

namespace nebula::threads {

    template <typename T, std::size_t MaxFrames>
    class BlockingQueue
    {
    public:
        BlockingQueue() : m_full(0), m_open(MaxFrames) {}

        BlockingQueue(BlockingQueue&&) = delete;
        BlockingQueue(const BlockingQueue&) = delete;
        BlockingQueue& operator = (BlockingQueue&&) = delete;
        BlockingQueue& operator = (const BlockingQueue&) = delete;

        void push(T&& item)
        {
            m_open.acquire();
            {
                std::lock_guard<std::mutex> lock{m_mutex};
                m_queue.emplace(std::move(item));
            }
            m_full.release();
        }

        template <typename Duration>
        std::optional<T> pop(const Duration& timeout)
        {
            std::optional<T> item;
            if (m_full.try_acquire_for(timeout))
            {
                std::lock_guard<std::mutex> lock{m_mutex};
                item = std::move(m_queue.front());
                m_queue.pop();
            }
            else
                return item;

            m_open.release();
            return item;
        }

        std::optional<T> pop(int timeout_ms = -1)
        {
            return pop(std::chrono::milliseconds(static_cast<uint32_t>(timeout_ms)));
        }

        [[nodiscard]] bool empty()
        {
            std::lock_guard<std::mutex> lock{m_mutex};
            return m_queue.empty();
        }

        [[nodiscard]] std::size_t size()
        {
            std::lock_guard<std::mutex> lock{m_mutex};
            return m_queue.size();
        }

    private:
        std::queue<T> m_queue;

        std::mutex m_mutex;
        std::counting_semaphore<MaxFrames> m_full;
        std::counting_semaphore<MaxFrames> m_open;
    };

}

#endif //BLOCKINGQUEUE_H
