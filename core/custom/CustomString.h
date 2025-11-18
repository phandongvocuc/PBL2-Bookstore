#pragma once

#include "core/custom/CaseSensitivity.h"

namespace custom {

class CustomString {
public:
    typedef unsigned int SizeType;

    CustomString();
    explicit CustomString(const char *text);
    CustomString(const char *text, SizeType length);
    CustomString(const CustomString &other);
    ~CustomString();

    CustomString &operator=(const CustomString &other);

    const char *cStr() const;
    char *data() const;
    SizeType length() const;
    bool isEmpty() const { return _length == 0U; }

    bool operator==(const CustomString &other) const { return compare(other) == 0; }
    bool operator!=(const CustomString &other) const { return !(*this == other); }

    void reserve(SizeType newCapacity);

    void append(const CustomString &other);
    void append(const char *text);
    void append(const char *text, SizeType length);

    const char &operator[](SizeType index) const;

    int compare(const CustomString &other) const;
    int compare(const CustomString &other, CaseSensitivity cs) const;
    int compare(const char *text, CaseSensitivity cs) const;

    CustomString trimmed() const;
    CustomString toUpper() const;

    bool startsWith(const CustomString &prefix, CaseSensitivity cs = CaseSensitivity::Sensitive) const;
    const unsigned char *bytes() const;

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

} // namespace custom
