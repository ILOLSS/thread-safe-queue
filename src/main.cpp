#include <unistd.h>
#include <atomic>
#include <iostream>
#include <string>
#include "../include/lock_guard.hpp"
#include "../include/thread_guard.hpp"
#include "../include/thread_safe_queue.hpp"

static ThreadSafeQueue<int> queue;

void producer() {
    for (int i = 0; i < 100; i++) {
        int x = std::rand() % 100;
        queue.push(x);
    }
}

void consumer(const std::atomic<bool> &is_stop) {
    while (!is_stop) {
        int x;
        if (!queue.try_pop(x))
            continue;
        bool flag = true;
        for (int i = 2; i * i <= x; i++) {
            if (x % i == 0) {
                flag = false;
                break;
            }
        }
        std::string ans = std::to_string(x);
        if (flag) {
            ans += " is prime\n";
        } else {
            ans += " is not prime\n";
        }
        std::cout << ans;
    }
}

int main() {
    std::atomic<bool> is_stop(false);

    ThreadGuard guard1(producer);
    ThreadGuard guard2(consumer, std::ref(is_stop));
    ThreadGuard guard3(consumer, std::ref(is_stop));
    ThreadGuard guard4(consumer, std::ref(is_stop));
    ThreadGuard guard5(consumer, std::ref(is_stop));
    ThreadGuard guard6(consumer, std::ref(is_stop));

    sleep(1);  // some code

    is_stop = true;
}