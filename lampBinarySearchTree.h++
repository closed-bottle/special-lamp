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

            // Undefined behavior if tree is empty but still tried to call min().
            // Check for assert only for debug build.
            LAMPASSERT(head_ != nullptr, "BinarySearchTree is empty.");

            while (curr->left_) {
                curr = curr->left_;
            }

            return curr;
        }

        node* max(node* _root) const {
            node* curr = _root;

            // Undefined behavior if tree is empty but still tried to call min().
            // Check for assert only for debug build.
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


        T min() const {return min(head_)->data_;}
        T max() const {return max(head_)->data_;}

        size_t size() const {
            return size_;
        }
    };
};

#endif //LAMPBINARYSEARCHTREE_H
