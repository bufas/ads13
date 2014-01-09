#ifndef INT_LIST_HPP_
#define INT_LIST_HPP_

struct IntList {
    IntList(int id, int size) : id_(id), head_(-1) {
        elements_ = new int[size];
    }

    ~IntList() {
        delete[] elements_;
    }

    int id() {
        return id_;
    }

    void push(int e) {
        head_++;
        elements_[head_] = e;
    }

    int removeAt(int index) {
        int element = elements_[index];
        elements_[index] = elements_[head_];
        head_--;
        return element;
    }

    void removeValue(int value) {
        for (int i = 0; i <= head_; i++) {
            if (elements_[i] == value) {
                elements_[i] = elements_[head_];
                head_--;
                return;
            }
        }
    }

    int at(int i) {
        return elements_[i];
    }

    int size() {
        return (head_ + 1);
    }

    bool isEmpty() {
        return head_ < 0;
    }

    int *elements_;
    int id_, head_;
};

#endif