#pragma once
#include "CustomContainers.h"
namespace pbl2::core {

// ======================= Vector =======================

template <typename T>
Vector<T>::Vector() : data_(nullptr), size_(0), capacity_(0) {}

template <typename T>
Vector<T>::Vector(const Vector &other) : data_(nullptr), size_(0), capacity_(0) {
    reserve(other.size_);
    for (int i = 0; i < other.size_; ++i)
        push_back(other.data_[i]);
}

template <typename T>
Vector<T> &Vector<T>::operator=(const Vector &other) {
    if (this != &other) {
        clear();
        reserve(other.size_);
        for (int i = 0; i < other.size_; ++i)
            push_back(other.data_[i]);
    }
    return *this;
}

template <typename T>
Vector<T>::~Vector() {
    delete[] data_;
}

template <typename T>
int Vector<T>::size() const {
    return size_;
}

template <typename T>
bool Vector<T>::isEmpty() const {
    return size_ == 0;
}

template <typename T>
void Vector<T>::clear() {
    size_ = 0;
}

template <typename T>
void Vector<T>::reserve(int count) {
    if (count <= capacity_) return;
    T *newData = new T[count];
    for (int i = 0; i < size_; ++i)
        newData[i] = data_[i];
    delete[] data_;
    data_ = newData;
    capacity_ = count;
}

template <typename T>
void Vector<T>::ensureCapacity(int minCap) {
    if (minCap <= capacity_) return;
    int newCap = (capacity_ == 0 ? 4 : capacity_ * 2);
    if (newCap < minCap) newCap = minCap;
    reserve(newCap);
}

template <typename T>
void Vector<T>::push_back(const T &value) {
    ensureCapacity(size_ + 1);
    data_[size_++] = value;
}

template <typename T>
void Vector<T>::append(const T &value) {
    push_back(value);
}

template <typename T>
T Vector<T>::value(int index, const T &defaultValue) const {
    return (index >= 0 && index < size_) ? data_[index] : defaultValue;
}

template <typename T>
const T &Vector<T>::at(int index) const {
    return data_[index];
}

template <typename T>
T &Vector<T>::operator[](int index) {
    return data_[index];
}

template <typename T>
const T &Vector<T>::operator[](const int index) const {
    return data_[index];
}

template <typename T>
typename Vector<T>::iterator Vector<T>::begin() { return data_; }

template <typename T>
typename Vector<T>::iterator Vector<T>::end() { return data_ + size_; }

template <typename T>
typename Vector<T>::const_iterator Vector<T>::begin() const { return data_; }

template <typename T>
typename Vector<T>::const_iterator Vector<T>::end() const { return data_ + size_; }


// ======================= Map =======================

template <typename K, typename V>
Map<K, V>::ConstIterator::ConstIterator(const Node *ptr) : ptr_(ptr) {}

template <typename K, typename V>
typename Map<K, V>::ConstIterator &
Map<K, V>::ConstIterator::operator++() {
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
Map<K, V>::Map() : data_(nullptr), size_(0), capacity_(0) {}

template <typename K, typename V>
Map<K, V>::~Map() {
    delete[] data_;
}

template <typename K, typename V>
int Map<K, V>::size() const {
    return size_;
}

template <typename K, typename V>
bool Map<K, V>::isEmpty() const {
    return size_ == 0;
}

template <typename K, typename V>
void Map<K, V>::clear() {
    size_ = 0;
}

template <typename K, typename V>
int Map<K, V>::findIndex(const K &key) const {
    for (int i = 0; i < size_; ++i)
        if (data_[i].key == key)
            return i;
    return -1;
}

template <typename K, typename V>
void Map<K, V>::ensureCapacity(int minCap) {
    if (minCap <= capacity_) return;
    int newCap = (capacity_ == 0 ? 4 : capacity_ * 2);
    if (newCap < minCap) newCap = minCap;
    reserve(newCap);
}

template <typename K, typename V>
void Map<K, V>::reserve(int count) {
    Node *newData = new Node[count];
    for (int i = 0; i < size_; ++i)
        newData[i] = data_[i];
    delete[] data_;
    data_ = newData;
    capacity_ = count;
}

template <typename K, typename V>
V &Map<K, V>::operator[](const K &key) {
    int idx = findIndex(key);
    if (idx >= 0)
        return data_[idx].value;

    ensureCapacity(size_ + 1);
    data_[size_].key = key;
    data_[size_].value = V();
    return data_[size_++].value;
}

template <typename K, typename V>
void Map<K, V>::insert(const K &key, const V &value) {
    int idx = findIndex(key);

    if (idx >= 0) {
        data_[idx].value = value;
        return;
    }

    ensureCapacity(size_ + 1);
    data_[size_].key = key;
    data_[size_].value = value;
    size_++;
}

template <typename K, typename V>
V Map<K, V>::value(const K &key, const V &defaultValue) const {
    int idx = findIndex(key);
    return idx >= 0 ? data_[idx].value : defaultValue;
}

template <typename K, typename V>
typename Map<K, V>::ConstIterator Map<K, V>::constBegin() const {
    return ConstIterator(data_);
}

template <typename K, typename V>
typename Map<K, V>::ConstIterator Map<K, V>::constEnd() const {
    return ConstIterator(data_ + size_);
}

} // namespace pbl2::core
