#ifndef LAMPLIST_H
#define LAMPLIST_H

namespace Lamp {

    template <typename T>
    class list {
        struct node {
            T data_;
            node* next_;
        };

        node* head_;
        node* tail_;
        uint64_t count_ = 0;
    public:
        list() : head_(nullptr), tail_(nullptr), count_(0) {}
        ~list() {
            while (!empty()) {
                node* new_head = head_->next_;
                delete head_;
                head_ = new_head;
            }
        }

        bool empty() const {
            return count_ == 0;
        }

        T& front() {
            return head_->data_;
        }

        const T & front() const {
            return head_->data_;
        }

        const T & back() const {
            return tail_->data_;
        }

        void push_front(const T & _value) {
            node* new_node = new node();
            new_node->data_ = _value;

            new_node->next_ = head_;
            head_ = new_node;

            if (count_ == 0) {
                tail_ = head_;
            }

            ++count_;
        }

        void push_back(const T & _value) {
            node* new_node = new node();
            new_node->data_ = _value;

            if (tail_) {
                tail_->next_ = new_node;
            }

            tail_ = new_node;

            if (count_ == 0) {
                head_ = tail_;
            }

            ++count_;
        }

        void pop_front() {
            // if (!head_)
            // TODO: runtime error
            node* new_head = head_->next_;
            delete head_;
            head_ = new_head;
            --count_;

            if (count_ == 1) {
                tail_ = head_;
            }
            if (count_ == 0) {
                tail_ = nullptr;
            }
        }

        void pop_back() {
            // if (!head)
            // TODO: runtime error
            node* prev = head_;

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
            }
            if (count_ == 0) {
                head_ = nullptr;
            }
        }

        void insert(uint64_t _pos, const T & _data) {
            if (_pos == 0) {
                node* new_node = new node;
                new_node->data_ = _data;
                new_node->next_ = head_;
                head_ = new_node;
                tail_ = new_node;
                ++count_;
                return;
            }
            else if (_pos == count_) {
                push_back(_data);
                return;
            }
            else if (_pos > count_) {
                //TODO: runtime error
            }

            node* prev = head_;
            uint64_t curr = 0;
            while (prev && curr < _pos - 1) {
                prev = prev->next_;
                curr++;
            }

            node* new_node = new node;
            new_node->data_ = _data;
            new_node->next_ = prev->next_;
            prev->next_ = new_node;


            ++count_;
        }
    };

}

#endif //LAMPLIST_H
