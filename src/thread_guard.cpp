#include "../include/thread_guard.hpp"
#include <iostream>

ThreadGuard::ThreadGuard(ThreadGuard &&other) {
    thread_ = std::move(other.thread_);
    is_joinable_ = other.is_joinable_;
    other.is_joinable_ = false;
}

ThreadGuard &ThreadGuard::operator=(ThreadGuard &&other) {
    thread_ = std::move(other.thread_);
    is_joinable_ = other.is_joinable_;
    other.is_joinable_ = false;
    return *this;
}

ThreadGuard::~ThreadGuard() {
    if (is_joinable_ == true) {
        thread_.join();
    }
}
