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

extern template class Map<CustomString, int>;

}  // namespace pbl2::core
