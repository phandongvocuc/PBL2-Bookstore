#pragma once

namespace pbl2::core {

struct NullOptTag {
};

constexpr NullOptTag nullopt{};

template <typename T>
class Optional {
public:
    Optional() : hasValue(false), storage() {}

    Optional(NullOptTag) : hasValue(false), storage() {}

    explicit Optional(const T &value) : hasValue(true), storage(value) {}

    Optional &operator=(const T &value) {
        storage = value;
        hasValue = true;
        return *this;
    }

    ~Optional() = default;

    [[nodiscard]] bool has_value() const {
        return hasValue;
    }

    T &value() {
        return storage;
    }

    const T &value() const {
        return storage;
    }

    const T &operator*() const {
        return storage;
    }

    const T *operator->() const {
        return &storage;
    }

private:
    bool hasValue;
    T storage;
};

}  // namespace pbl2::core
