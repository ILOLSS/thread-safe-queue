#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>
#include "lock_guard.hpp"

template <typename T>
class ThreadSafeQueue {
    typedef T type_name;

public:
    void push(const type_name &value) {
        LockGuard lock(mutex_);
        queue_.push(value);
        isNotQueueEmpty_.notify_all();
    }

    void push(type_name &&value) {
        LockGuard lock(mutex_);
        queue_.push(std::forward<type_name>(value));
        isNotQueueEmpty_.notify_all();
    }

    bool try_pop(type_name &value) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (!isNotQueueEmpty_.wait_for(
                lock, std::chrono::milliseconds(100),
                [this]() { return !queue_.empty(); }
            )) {
            return false;
        }
        value = std::move(queue_.front());
        queue_.pop();
        return true;
    }

private:
    std::queue<type_name> queue_;
    std::mutex mutex_;
    std::condition_variable isNotQueueEmpty_;
};

#endif  // THREAD_SAFE_QUEUE_H