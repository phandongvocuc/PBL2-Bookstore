#pragma once

#include <vector>
#include <map>

// Lightweight stand-ins for Qt containers so the codebase can avoid using Qt's
// QVector/QMap while keeping the existing APIs.
namespace custom {

template <typename T>
class Vector {
public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    Vector() = default;
    Vector(std::initializer_list<T> init) : data_(init) {}

    int size() const { return static_cast<int>(data_.size()); }
    bool isEmpty() const { return data_.empty(); }
    void clear() { data_.clear(); }

    T value(int index, const T &defaultValue = T()) const {
        const auto idx = static_cast<std::size_t>(index);
        return idx < data_.size() ? data_[idx] : defaultValue;
    }

    const T &at(int index) const { return data_.at(static_cast<std::size_t>(index)); }

    void reserve(int count) { data_.reserve(static_cast<std::size_t>(count)); }
    void append(const T &value) { data_.push_back(value); }
    void push_back(const T &value) { data_.push_back(value); }
    void pushBack(const T &value) { data_.push_back(value); }

    T &operator[](int index) { return data_[static_cast<std::size_t>(index)]; }
    const T &operator[](int index) const { return data_[static_cast<std::size_t>(index)]; }

    iterator begin() { return data_.begin(); }
    const_iterator begin() const { return data_.begin(); }
    const_iterator cbegin() const { return data_.cbegin(); }

    iterator end() { return data_.end(); }
    const_iterator end() const { return data_.end(); }
    const_iterator cend() const { return data_.cend(); }

private:
    std::vector<T> data_;
};

template <typename K, typename V>
class Map {
public:
    class ConstIterator {
    public:
        explicit ConstIterator(typename std::map<K, V>::const_iterator it) : it_(it) {}
        ConstIterator &operator++() {
            ++it_;
            return *this;
        }
        bool operator!=(const ConstIterator &other) const { return it_ != other.it_; }
        const K &key() const { return it_->first; }
        const V &value() const { return it_->second; }

    private:
        typename std::map<K, V>::const_iterator it_;
    };

    Map() = default;

    V &operator[](const K &key) { return data_[key]; }

    V value(const K &key, const V &defaultValue = V()) const {
        const auto it = data_.find(key);
        return it == data_.end() ? defaultValue : it->second;
    }

    void insert(const K &key, const V &value) { data_[key] = value; }

    int size() const { return static_cast<int>(data_.size()); }
    bool isEmpty() const { return data_.empty(); }
    void clear() { data_.clear(); }

    ConstIterator constBegin() const { return ConstIterator(data_.cbegin()); }
    ConstIterator constEnd() const { return ConstIterator(data_.cend()); }

private:
    std::map<K, V> data_;
};

}  // namespace custom
