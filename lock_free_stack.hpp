// Copyright Davydov Nikolay 2021

#ifndef INCLUDE_LOCK_FREE_STACK_HPP_
#define INCLUDE_LOCK_FREE_STACK_HPP_

#include <atomic>
#include <memory>

namespace lock_free_stack{
    template<typename T>
    class LockFreeStack {
    private:
        struct Node {
            T val_;
            std::shared_ptr<Node> next_;

            Node() = default;

            explicit Node(const T &val) : val_(val), next_(nullptr) {}
        };

        std::shared_ptr<Node> head = nullptr;

    public:
        void Push(const T &val) {
            std::shared_ptr<Node> new_node = std::make_shared<Node>(val);
            new_node->next_ = std::atomic_load(&head);

            while (!std::atomic_compare_exchange_weak(&head, &new_node->next_, new_node));
        }

        bool TryPop(T &value) {
            if (!head) {
                return false;
            }

            std::shared_ptr<Node> old_head = std::atomic_load(&head);
            while (!std::atomic_compare_exchange_weak(&head, &old_head, old_head->next_)) {
                if (!old_head) {
                    return false;
                }
            }

            value = old_head->val_;
            return true;
        }
    };
}


#endif //INCLUDE_LOCK_FREE_STACK_HPP_
