#ifndef VEB_HEAP_CPP_
#define VEB_HEAP_CPP_

#include <iostream>
#include <cmath>
#include <vector>
#include "heap.hpp"
#include "node.hpp"

using std::cout;
using std::endl;

/**
 * Forward declarations
 */
template<typename T> class VebTree;
template<typename T> class VebTreeNode;
template<typename T> class VebTreeLeaf;

template<typename T> class VebNode;
template<typename T> class VebHeap;

/**
 * vEB tree implementation
 */
template<typename T>
class VebTree {
    public:
        virtual ~VebTree() {}

        virtual void insert(int, VebNode<T>*) = 0;
        virtual VebNode<T>* remove(int) = 0;
        virtual VebNode<T>* get(int) = 0;
        virtual VebNode<T>* findMin() = 0;
        virtual VebNode<T>* deleteMin() = 0;

        virtual int size() = 0;
        virtual int max() = 0;
        virtual int min() = 0;
};

// u = 2
template<typename T>
class VebTreeLeaf : public VebTree<T> {
    public:
        VebTreeLeaf() : size_(0), max_(-1), min_(2) {
        }

        ~VebTreeLeaf() {
            nodes_[0].clear();
            nodes_[1].clear();
        }

        void insert(int key, VebNode<T> *n) {
            nodes_[key].push_back(n);

            size_ += 1;
            switch (size_) {
                case 1:
                    max_ = min_ = key;
                    break;
                default:
                    min_ = !nodes_[0].empty() ? 0 : 1;
                    max_ = !nodes_[1].empty() ? 1 : 0;
                    break;
            }
        }

        VebNode<T>* remove(int key) {
            VebNode<T> *n = nodes_[key].back();
            nodes_[key].pop_back();

            size_ -= 1;
            switch (size_) {
                case 0:
                    max_ = -1;
                    min_ = 2;
                    break;
                case 1:
                    max_ = min_ = !nodes_[key].empty() ? key : 1 - key;
                    break;
                default:
                    min_ = !nodes_[0].empty() ? 0 : 1;
                    max_ = !nodes_[1].empty() ? 1 : 0;
                    break;
            }

            return n;
        }

        VebNode<T>* get(int key) {
            return nodes_[key].back();
        }

        VebNode<T>* findMin() {
            int min = this->min();
            return (min < 0) ? NULL : get(min);
        }

        VebNode<T>* deleteMin() {
            int min = this->min();
            return (min < 0) ? NULL : remove(min);
        }

        int size() {
            return size_;
        }

        int max() {
            return max_;
        }

        int min() {
            return min_;
        }

    private:
        int size_;
        int max_;
        int min_;

        std::vector<VebNode<T>*> nodes_[2];
};

// u > 2
template<typename T>
class VebTreeNode : public VebTree<T> {
    public:
        VebTreeNode(int u) : u_(u), sqrtu_(1), size_(0), max_(-1), min_(u) {
            float root = std::sqrt(u);
            while (sqrtu_ < root) {
                sqrtu_ *= 2;
            }

            if (sqrtu_ == 2) {
                summary_ = new VebTreeLeaf<T>();
                cluster_ = new VebTree<T>*[2];
                cluster_[0] = new VebTreeLeaf<T>();
                cluster_[1] = new VebTreeLeaf<T>();
            } else {
                summary_ = NULL;
                cluster_ = new VebTree<T>*[sqrtu_];
                for (int i = 0; i < sqrtu_; i++) {
                    cluster_[i] = NULL;
                }
            }
        }

        ~VebTreeNode() {
            delete summary_;
            summary_ = NULL;

            for (int i = 0; i < sqrtu_; i++) {
                delete cluster_[i];
                cluster_[i] = NULL;
            }
            delete[] cluster_;
            cluster_ = NULL;
        }

        void insert(int i, VebNode<T> *n) {
            int a = i / sqrtu_;
            int b = i % sqrtu_;

            if (cluster_[a] == NULL) {
                cluster_[a] = new VebTreeNode<T>(sqrtu_);
            }
            if (cluster_[a]->size() == 0) {
                if (summary_ == NULL) {
                    summary_ = new VebTreeNode<T>(sqrtu_);
                }
                summary_->insert(a, n);
            }
            cluster_[a]->insert(b, n);

            size_ += 1;
            if (i > max_) max_ = i;
            if (i < min_) min_ = i;
        }

        VebNode<T>* remove(int i) {
            int a = i / sqrtu_;
            int b = i % sqrtu_;

            VebNode<T> *n = cluster_[a]->remove(b);
            if (cluster_[a]->size() == 0) {
                delete cluster_[a];
                cluster_[a] = NULL;

                summary_->remove(a);
                if (summary_->size() == 0) {
                    delete summary_;
                    summary_ = NULL;
                }
            }

            size_ -= 1;
            if (size_ >= 1) {
                if (i == min_) {
                    a = summary_->min();
                    b = cluster_[a]->min();
                    min_ = a * sqrtu_ + b;
                }
                if (i == max_) {
                    a = summary_->max();
                    b = cluster_[a]->max();
                    max_ = a * sqrtu_ + b;
                }
            }
            
            return n;
        }

        VebNode<T>* get(int i) {
            int a = i / sqrtu_;
            int b = i % sqrtu_;

            return cluster_[a]->get(b);
        }

        VebNode<T>* findMin() {
            return (min_ < 0) ? NULL : get(min_);
        }

        VebNode<T>* deleteMin() {
            return (min_ < 0) ? NULL : remove(min_);
        }

        int size() {
            return size_;
        }

        int max() {
            return max_;
        }

        int min() {
            return min_;
        }

    private:
        int u_;
        int sqrtu_;

        int size_;
        int max_;
        int min_;

        VebTree<T> *summary_;
        VebTree<T> **cluster_;
};

/**
 * Internal node used by the vEB heap implementation.
 */
template<typename T>
class VebNode : public Node<T> {
    public:
        VebNode(const int key, const T value) : key_(key), value_(value) {
        }

        ~VebNode() {
        }

        int getKey() {
            return key_;
        }

        T getValue() {
            return value_;
        }

        friend class VebHeap<T>;

    private:
        int key_;
        const T value_;
};


/**
 * vEB heap wrapper declaration
 */
template<typename T>
class VebHeap : public Heap<T> {
    public:
        VebHeap(int u) : size_(0) {
            veb_ = new VebTreeNode<T>(u);
        }

        ~VebHeap() {
            delete veb_;
        }

        Node<T>* insert(int key, T value) {
            VebNode<T> *n = new VebNode<T>(key, value);
            veb_->insert(key, n);
            size_ += 1;
            return n;
        }

        Node<T>* findMin() {
            return veb_->findMin();
        }

        Node<T>* deleteMin() {
            Node<T> *min = veb_->deleteMin();
            if (min != NULL) {
                size_ -= 1;
            }
            return min;
        }

        void decreaseKey(Node<T> *n, int delta) {
            VebNode<T> *vn = (VebNode<T> *) n;
            veb_->remove(vn->key_);
            veb_->insert(vn->key_ - delta, vn);
        }

        int size() {
            return size_;
        }

    private:
        VebTree<T> *veb_;
        int size_;
};

#endif