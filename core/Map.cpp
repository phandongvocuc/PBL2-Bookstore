#include "Map.h"
#include "CustomString.h"

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

template class Map<CustomString, int>;

}  // namespace pbl2::core
