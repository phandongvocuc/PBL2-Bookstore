#pragma once

namespace pbl2::core {

    // ======================= Vector =======================
    template <typename T>
    class Vector {
    public:
        using iterator = T*;
        using const_iterator = const T*;

        Vector();
        Vector(const Vector &other);
        Vector &operator=(const Vector &other);
        ~Vector();

        int  size() const;
        bool isEmpty() const;
        void clear();

        void reserve(int count);
        void push_back(const T &value);
        void append(const T &value);

        T value(int index, const T &defaultValue = T()) const;
        const T &at(int index) const;

        T &operator[](int index);
        const T &operator[](int index) const;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end()   const;

    private:
        void ensureCapacity(int minCap);

        T  *data_;
        int size_;
        int capacity_;
    };

    // ======================= Map =======================
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
            ConstIterator(const Node *ptr);
            ConstIterator &operator++();
            bool operator!=(const ConstIterator &other) const;

            const K &key() const;
            const V &value() const;

        private:
            const Node *ptr_;
        };

        Map();
        ~Map();

        int  size() const;
        bool isEmpty() const;
        void clear();

        V &operator[](const K &key);
        void insert(const K &key, const V &value);
        V value(const K &key, const V &defaultValue = V()) const;

        ConstIterator constBegin() const;
        ConstIterator constEnd()   const;

    private:
        int findIndex(const K &key) const;
        void ensureCapacity(int minCap);
        void reserve(int count);

        Node *data_;
        int size_;
        int capacity_;
    };

} // namespace pbl2::core

#include "CustomContainers.cpp"   // include template implementations
