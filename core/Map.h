#pragma once

#include "DynamicArray.h"
namespace pbl2::core {

template <typename K, typename V>
class Map {
private:
    struct Node {
        K key;
        V value;
    };

public:
    class ConstIterator {
    public:
        explicit ConstIterator(const Node *ptr);
        ConstIterator &operator++();
        bool operator!=(const ConstIterator &other) const;
        const K &key() const;
        const V &value() const;

    private:
        const Node *ptr_;
    };

    Map();
    ~Map();

    [[nodiscard]] int size() const;
    [[nodiscard]] bool isEmpty() const;
    void clear();

    V &operator[](const K &key);
    void insert(const K &key, const V &value);
    V value(const K &key, const V &defaultValue = V()) const;

    ConstIterator constBegin() const;
    ConstIterator constEnd() const;

private:
    int findIndex(const K &key) const;

    DynamicArray<Node> entries_;
};

}  // namespace pbl2::core

// Template implementations
namespace pbl2::core {

template <typename K, typename V>
Map<K, V>::ConstIterator::ConstIterator(const Node *ptr) : ptr_(ptr) {}

template <typename K, typename V>
typename Map<K, V>::ConstIterator &Map<K, V>::ConstIterator::operator++() {
    ++ptr_;
    return *this;
}

template <typename K, typename V>
bool Map<K, V>::ConstIterator::operator!=(const ConstIterator &other) const {
    return ptr_ != other.ptr_;
}

template <typename K, typename V>
const K &Map<K, V>::ConstIterator::key() const {
    return ptr_->key;
}

template <typename K, typename V>
const V &Map<K, V>::ConstIterator::value() const {
    return ptr_->value;
}

template <typename K, typename V>
Map<K, V>::Map() = default;

template <typename K, typename V>
Map<K, V>::~Map() = default;

template <typename K, typename V>
int Map<K, V>::size() const {
    return static_cast<int>(entries_.size());
}

template <typename K, typename V>
bool Map<K, V>::isEmpty() const {
    return entries_.isEmpty();
}

template <typename K, typename V>
void Map<K, V>::clear() {
    entries_.clear();
}

template <typename K, typename V>
V &Map<K, V>::operator[](const K &key) {
    const int idx = findIndex(key);
    if (idx >= 0) {
        return entries_[static_cast<typename DynamicArray<Node>::SizeType>(idx)].value;
    }
    entries_.pushBack(Node{key, V()});
    return entries_[entries_.size() - 1U].value;
}

template <typename K, typename V>
void Map<K, V>::insert(const K &key, const V &value) {
    const int idx = findIndex(key);
    if (idx >= 0) {
        entries_[static_cast<typename DynamicArray<Node>::SizeType>(idx)].value = value;
        return;
    }
    entries_.pushBack(Node{key, value});
}

template <typename K, typename V>
V Map<K, V>::value(const K &key, const V &defaultValue) const {
    const int idx = findIndex(key);
    return idx >= 0 ? entries_[static_cast<typename DynamicArray<Node>::SizeType>(idx)].value : defaultValue;
}

template <typename K, typename V>
typename Map<K, V>::ConstIterator Map<K, V>::constBegin() const {
    return ConstIterator(entries_.begin());
}

template <typename K, typename V>
typename Map<K, V>::ConstIterator Map<K, V>::constEnd() const {
    return ConstIterator(entries_.end());
}

template <typename K, typename V>
int Map<K, V>::findIndex(const K &key) const {
    for (typename DynamicArray<Node>::SizeType i = 0U; i < entries_.size(); ++i) {
        if (entries_[i].key == key) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

}  // namespace pbl2::core
