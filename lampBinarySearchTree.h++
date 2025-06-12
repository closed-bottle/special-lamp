//
// Created by JJ on 6/9/2025.
//

#ifndef LAMPBINARYSEARCHTREE_H
#define LAMPBINARYSEARCHTREE_H

#include "lampQueue.h++"
#include "lampUtility.h++"

namespace Lamp {
    template<typename T>
    class BinarySearchTree {
        struct node {
            T data_{};
            node* left_ = nullptr;
            node* right_ = nullptr;
        };
        node* head_ = nullptr;
        size_t size_ = 0;

        node* min(node* _root) const {
            node* curr = _root;

            LAMPASSERT(head_ != nullptr, "BinarySearchTree is empty.");

            while (curr->left_) {
                curr = curr->left_;
            }

            return curr;
        }

        node* max(node* _root) const {
            node* curr = _root;

            LAMPASSERT(head_ != nullptr, "BinarySearchTree is empty.");

            while (curr->right_) {
                curr = curr->right_;
            }

            return curr;
        }

    public:
        ~BinarySearchTree() {
            clear();
        }

        void clear() {
            queue<node*> q; // breadth first search to prevent stack overflow.
            q.push(head_);

            while (!q.empty()) {
                node* curr = q.top();
                q.pop();

                if (!curr) {
                    continue;
                }

                q.push(curr->left_);
                q.push(curr->right_);
                delete curr;
            }

            size_ = 0;
        }

        void insert(const T& _in) {
            node** curr = &head_;

            while (true) {
                if (!(*curr)) {
                    (*curr) = new node{_in, nullptr, nullptr};
                    break;
                }

                if (_in < (*curr)->data_) {
                    curr = &(*curr)->left_;
                }
                else {
                    curr = &(*curr)->right_;
                }
            }

            ++size_;
        }

        bool exist(const T& _in) const {
            node* curr = head_;

            LAMPASSERT(head_ != nullptr, "BinarySearchTree is empty.");

            while (curr) {
                if (_in == curr->data_)
                    return true;

                if (_in < curr->data_) {
                    curr = curr->left_;
                }
                else {
                    curr = curr->right_;
                }
            }

            return false;
        }

        // Changed to iterative solution, if you want recursive version
        // goto:
        // b8d1e1576904e0feccf0ba762085926a1efe41a6
        void erase(const T& _in) {
            LAMPASSERT(head_ != nullptr, "BinarySearchTree is empty.");
            if (!head_) {
                return;
            }

            node* prev = nullptr;
            node* root = head_;

            while (root && root->data_ != _in) {
                prev = root;
                root = root->data_ < _in ? root->left_ : root->right_;
            }

            // unable to find one
            if (!root)
                return;

            // one or no child node at all.
            if (!root->left_ || !root->right_) {
                node* curr = root;

                if (root->left_)
                    curr = root->left_;
                else
                    curr = root->right_;

                // prev is null if and only if root == head_.(first iteration)
                if (!prev) {
                    delete head_;
                    head_ = curr;
                }

                if (root == prev->left_)
                    prev->left_ = curr;
                else
                    prev->right_ = curr;

                delete root;
            }
            else {
                node* curr = root->right_;
                while (curr && curr->left_) {
                    prev = curr;
                    curr = curr->left_;
                }

                if (prev)
                    prev->left_ = curr->right_;
                else
                    root->right_ = curr->right_;

                root->data_ = std::move(curr->data_);
                delete curr;
            }

            head_ = root;

            --size_;
        }

        T min() const {return min(head_)->data_;}
        T max() const {return max(head_)->data_;}

        size_t size() const {
            return size_;
        }
    };
};

#endif //LAMPBINARYSEARCHTREE_H
