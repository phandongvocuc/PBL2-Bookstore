#pragma once

#include <initializer_list>

using namespace std;
namespace pbl2::core {

template <typename T>
class DynamicArray {
public:
    typedef unsigned int SizeType;

    typedef T *Iterator;
    typedef const T *ConstIterator;

    DynamicArray() : _data(nullptr), _size(0), _capacity(0) {}

    explicit DynamicArray(const SizeType initialSize, const T &value = T()) : _data(nullptr), _size(0), _capacity(0) {
        if (initialSize > 0U) {
            reserve(initialSize);
            for (SizeType i = 0; i < initialSize; ++i) {
                _data[i] = value;
            }
            _size = initialSize;
        }
    }

    DynamicArray(initializer_list<T> init) : _data(nullptr), _size(0), _capacity(0) {
        reserve(static_cast<SizeType>(init.size()));
        for (const auto &value : init) {
            pushBack(value);
        }
    }

    DynamicArray(const DynamicArray &other) : _data(nullptr), _size(0), _capacity(0) {
        copyFrom(other);
    }

    ~DynamicArray() {
        destroy();
    }

    DynamicArray &operator=(const DynamicArray &other) {
        if (this != &other) {
            copyFrom(other);
        }
        return *this;
    }

    T &operator[](SizeType index) {
        return _data[index];
    }

    const T &operator[](SizeType index) const {
        return _data[index];
    }

    const T &at(SizeType index) const {
        return _data[index];
    }

    void pushBack(const T &value) {
        ensureCapacity(_size + 1U);
        _data[_size] = value;
        ++_size;
    }

    void push_back(const T &value) {
        pushBack(value);
    }

    void append(const T &value) {
        pushBack(value);
    }

    void removeAt(const SizeType index) {
        if (index >= _size) {
            return;
        }
        for (SizeType i = index; i + 1U < _size; ++i) {
            _data[i] = _data[i + 1U];
        }
        --_size;
    }

    void clear() {
        _size = 0U;
    }

    void reserve(const SizeType newCapacity) {
        if (newCapacity <= _capacity) {
            return;
        }
        reallocate(newCapacity);
    }

    void resize(const SizeType newSize, const T &value = T()) {
        if (newSize > _capacity) {
            reallocate(newSize);
        }
        if (newSize > _size) {
            for (SizeType i = _size; i < newSize; ++i) {
                _data[i] = value;
            }
        }
        _size = newSize;
    }

    [[nodiscard]] SizeType size() const {
        return _size;
    }

    [[nodiscard]] bool isEmpty() const {
        return _size == 0U;
    }

    [[nodiscard]] SizeType capacity() const {
        return _capacity;
    }

    T *data() {
        return _data;
    }

    const T *data() const {
        return _data;
    }

    Iterator begin() {
        return _data;
    }

    ConstIterator begin() const {
        return _data;
    }

    Iterator end() {
        return _data ? _data + _size : nullptr;
    }

    ConstIterator end() const {
        return _data ? _data + _size : nullptr;
    }

    T value(const SizeType index, const T &defaultValue = T()) const {
        return index < _size ? _data[index] : defaultValue;
    }

private:
    void ensureCapacity(const SizeType needed) {
        if (needed <= _capacity) {
            return;
        }
        SizeType newCapacity = _capacity == 0U ? 4U : _capacity * 2U;
        if (newCapacity < needed) {
            newCapacity = needed;
        }
        reallocate(newCapacity);
    }

    void copyFrom(const DynamicArray &other) {
        if (other._size == 0U) {
            destroy();
            return;
        }
        _size = 0U;
        reallocate(other._size);
        for (SizeType i = 0; i < other._size; ++i) {
            _data[i] = other._data[i];
        }
        _size = other._size;
    }

    void reallocate(const SizeType newCapacity) {
        T *newData = new T[newCapacity];
        for (SizeType i = 0; i < _size; ++i) {
            newData[i] = _data[i];
        }
        delete[] _data;
        _data = newData;
        _capacity = newCapacity;
    }

    void destroy() {
        delete[] _data;
        _data = 0;
        _size = 0U;
        _capacity = 0U;
    }

    T *_data;
    SizeType _size;
    SizeType _capacity;
};

} // namespace pbl2::core
