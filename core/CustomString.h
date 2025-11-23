#pragma once

#include "CaseSensitivity.h"

namespace pbl2::core {

class CustomString {
public:
    typedef unsigned int SizeType;

    CustomString();
    explicit CustomString(const char *text);
    CustomString(const char *text, SizeType length);
    CustomString(const CustomString &other);
    ~CustomString();

    CustomString &operator=(const CustomString &other);

    [[nodiscard]] const char *cStr() const;
    [[nodiscard]] char *data() const;
    [[nodiscard]] SizeType length() const;
    [[nodiscard]] bool isEmpty() const { return _length == 0U; }

    bool operator==(const CustomString &other) const { return compare(other) == 0; }
    bool operator!=(const CustomString &other) const { return !(*this == other); }

    void reserve(SizeType newCapacity);

    void append(const CustomString &other);
    void append(const char *text);
    void append(const char *text, SizeType length);

    const char &operator[](SizeType index) const;

    [[nodiscard]] int compare(const CustomString &other) const;
    [[nodiscard]] int compare(const CustomString &other, CaseSensitivity cs) const;
    int compare(const char *text, CaseSensitivity cs) const;

    [[nodiscard]] CustomString trimmed() const;
    [[nodiscard]] CustomString toUpper() const;

    [[nodiscard]] bool startsWith(const CustomString &prefix, CaseSensitivity cs = CaseSensitivity::Sensitive) const;
    [[nodiscard]] const unsigned char *bytes() const;

private:
    char *_data;
    SizeType _length;
    SizeType _capacity;

    void ensureCapacity(SizeType needed);
    static SizeType stringLength(const char *text);
    static void copyCharacters(char *destination, const char *source, SizeType count);
    static int charCompare(char lhs, char rhs, CaseSensitivity cs);
    static bool isSpace(char ch);
    static char toLowerChar(char ch);
    static char toUpperChar(char ch);
};

inline CustomString CustomStringLiteral(const char *text) { return CustomString(text); }

}  // namespace pbl2::core
