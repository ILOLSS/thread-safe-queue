#ifndef LOCK_GUARD_H
#define LOCK_GUARD_H

#include <mutex>

class LockGuard {
public:
    explicit LockGuard(std::mutex &mutex) : mutex_(mutex) {
        mutex_.lock();
    }

    LockGuard(const LockGuard &) = delete;
    LockGuard &operator=(const LockGuard &) = delete;

    ~LockGuard() {
        mutex_.unlock();
    }

private:
    std::mutex &mutex_;
};

#endif  // LOCK_GUARD_H
