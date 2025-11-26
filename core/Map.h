#pragma once

#include "DynamicArray.h"
#include "CustomString.h"

namespace pbl2::core {

template <typename K, typename V>
class Map {
private:
    struct Node {
        K key;
        V value;
    };

    using SizeType = typename DynamicArray<Node>::SizeType;

public:
    class ConstIterator {
    public:
        explicit ConstIterator(const Node *ptr) : ptr_(ptr) {}

        ConstIterator &operator++() {
            if (ptr_) ++ptr_;
            return *this;
        }

        bool operator!=(const ConstIterator &other) const { return ptr_ != other.ptr_; }
        const K &key() const { return ptr_->key; }
        const V &value() const { return ptr_->value; }

    private:
        const Node *ptr_;
    };

    Map() = default;
    ~Map() = default;

    [[nodiscard]] int size() const { return static_cast<int>(entries_.size()); }
    [[nodiscard]] bool isEmpty() const { return entries_.isEmpty(); }
    void clear() { entries_.clear(); }

    V &operator[](const K &key) {
        const int idx = findIndex(key);
        if (idx >= 0) {
            return entries_[static_cast<SizeType>(idx)].value;
        }

        Node node{key, V()};
        entries_.append(node);
        return entries_[entries_.size() - 1U].value;
    }

    void insert(const K &key, const V &value) {
        const int idx = findIndex(key);
        if (idx >= 0) {
            entries_[static_cast<SizeType>(idx)].value = value;
            return;
        }
        entries_.append(Node{key, value});
    }

    V value(const K &key, const V &defaultValue = V()) const {
        const int idx = findIndex(key);
        return idx >= 0 ? entries_[static_cast<SizeType>(idx)].value : defaultValue;
    }

    ConstIterator constBegin() const { return ConstIterator(entries_.data()); }
    ConstIterator constEnd() const {
        const Node *data = entries_.data();
        return ConstIterator(data ? data + entries_.size() : nullptr);
    }

private:
    int findIndex(const K &key) const {
        const auto size = entries_.size();
        for (SizeType i = 0; i < size; ++i) {
            if (entries_[i].key == key) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    DynamicArray<Node> entries_;
};

}  // namespace pbl2::core
