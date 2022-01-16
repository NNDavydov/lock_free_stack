// Copyright Davydov Nikolay 2021
#include <string>
#include <iostream>
#include <thread>

#include "lock_free_stack.hpp"

using namespace lock_free_stack;

void foo(std::size_t begin, std::size_t end, LockFreeStack<std::size_t> &stack) {
    for (size_t i = begin; i < end; ++i) {
        stack.Push(i);
    }

    size_t x;
    while (stack.TryPop(x)) {
        std::cout << x << "\n";
    }
}

int main() {
    LockFreeStack<std::size_t> stack;
    std::thread t1(foo, 0, 100, std::ref(stack));
    std::thread t2(foo, 1000, 1100, std::ref(stack));
    std::thread t3(foo, 2300, 2500, std::ref(stack));

    t1.join();
    t2.join();
    t3.join();
    exit(EXIT_SUCCESS);
}
