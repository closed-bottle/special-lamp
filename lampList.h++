#ifndef LAMPLIST_H
#define LAMPLIST_H

#include <stdexcept>

namespace Lamp {
    template<typename T>
    class list {
        template<typename, typename>
        friend class unordered_map;

        struct node {
            template<typename, typename>
            friend class unordered_map;

            T data_;
            node *next_;

            node *next() {
                return next_;
            }
        };

        node *head_ = nullptr;
        node *tail_ = nullptr;
        uint64_t count_ = 0;

    public:
        list() : head_(nullptr), tail_(nullptr), count_(0) {
        }

        ~list() {
            while (!empty()) {
                node *new_head = head_->next_;
                delete head_;
                head_ = new_head;
                --count_;
            }
        }

        bool empty() const {
            return count_ == 0;
        }

        T &front() {
            return head_->data_;
        }

        T &back() {
            return tail_->data_;
        }

        const T &front() const {
            return head_->data_;
        }

        const T &back() const {
            return tail_->data_;
        }

        void push_front(const T &_value) {
            node *new_node = new node();
            new_node->data_ = _value;

            new_node->next_ = head_;
            head_ = new_node;

            if (count_ == 0) {
                tail_ = head_;
            }

            ++count_;
        }

        void push_back(const T &_value) {
            node *new_node = new node(_value, nullptr);

            if (tail_) {
                tail_->next_ = new_node;
            }

            tail_ = new_node;

            if (count_ == 0) {
                head_ = tail_;
            }

            ++count_;
        }

        T *at(uint64_t _pos) {
            node *n = head_;

            while (n && _pos) {
                n = n->next_;
                --_pos;
            }

            return &n->data_;
        }

        // _start == _end will erase single element.
        // Implementing this way so it's not confusing in case T == uint64
        void erase(uint64_t _start, uint64_t _size) {
            // Do not handle _start + _count > count_.
            if (_start == 0 && count_) {
                if (head_ == tail_) {
                    tail_ = nullptr;
                }

                while (_size) {
                    node *n = head_->next_;

                    delete head_;
                    head_ = n;
                    --_size;
                    --count_;
                }

                return;
            }

            node *prev = head_;

            while (prev && (_start - 1)) {
                prev = prev->next_;
                --_start;
            }


            node *curr = prev->next_;
            while (_size) {
                node *n = curr->next_;

                delete curr;
                curr = n;

                --_size;
                --count_;
            }
            prev->next_ = curr;

            if (!prev->next_) {
                tail_ = prev;
            }
        }

        void erase(const T &_value) {
            node *curr = head_;
            node *prev = nullptr;

            while (curr && curr->data_ != _value) {
                prev = curr;
                curr = curr->next_;
            }

            if (curr == head_) {
                if (head_ == tail_) {
                    tail_ = nullptr; // count_ == 0 edge case.
                }
                head_ = head_->next_;
                --count_;
                delete curr;
            } else if (curr == tail_) {
                if (head_ == tail_) {
                    head_ = nullptr;
                }
                tail_ = prev;
                --count_;
                delete curr;
            } else if (curr != nullptr) {
                prev->next_ = curr->next_;
                --count_;
                delete curr;
            }
        }

        void pop_front() {
            if (!head_) {
                // Ignore pop_front if list is empty.
                return;
            }

            node *new_head = head_->next_;
            delete head_;
            head_ = new_head;
            --count_;

            if (count_ == 1) {
                tail_ = head_;
            } else if (count_ == 0) {
                tail_ = nullptr;
            }
        }

        void pop_back() {
            if (!head_) {
                // Ignore pop_back if list is empty
                return;
            }
            node *prev = head_;

            while (prev && prev->next_ != tail_) {
                prev = prev->next_;
            }

            delete tail_;
            tail_ = prev;
            if (tail_) {
                tail_->next_ = nullptr;
            }
            --count_;

            if (count_ == 1) {
                head_ = tail_;
            } else if (count_ == 0) {
                head_ = nullptr;
            }
        }

        void insert(uint64_t _pos, const T &_data) {
            if (_pos == 0) {
                node *new_node = new node;
                new_node->data_ = _data;
                new_node->next_ = head_;
                head_ = new_node;
                tail_ = new_node;
                ++count_;
                return;
            } else if (_pos == count_) {
                push_back(_data);
                return;
            } else if (_pos > count_) {
                // Return if pos is out of bound instead of creating empty elements.
                return;
            }

            node *prev = head_;
            uint64_t curr = 0;
            while (prev && curr < _pos - 1) {
                prev = prev->next_;
                curr++;
            }

            node *new_node = new node;
            new_node->data_ = _data;
            new_node->next_ = prev->next_;
            prev->next_ = new_node;


            ++count_;
        }

        T *find(const T &_rhs, bool (*is_same)(const T &, const T &)) {
            node *curr = head_;
            while (curr) {
                if (is_same(curr, _rhs)) {
                    return curr->data_;
                }
                curr = curr->next_;
            }

            return nullptr;
        }

        uint64_t size() const {
            return count_;
        }
    };
}

#endif //LAMPLIST_H
