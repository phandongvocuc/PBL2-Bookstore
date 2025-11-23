#include "CustomString.h"

#include <cctype>
using namespace std;
namespace pbl2::core {

namespace {
constexpr char kEmptyString[] = "";
}  // namespace

CustomString::CustomString() : _data(nullptr), _length(0U), _capacity(0U) {
    reserve(8U);
    _data[0] = '\0';
}

CustomString::CustomString(const char *text) : CustomString() {
    const SizeType textLength = stringLength(text);
    ensureCapacity(textLength + 1U);
    copyCharacters(_data, text, textLength);
    _length = textLength;
    _data[_length] = '\0';
}

CustomString::CustomString(const char *text, const SizeType length) : CustomString() {
    if (!text || length == 0U) {
        return;
    }
    ensureCapacity(length + 1U);
    copyCharacters(_data, text, length);
    _length = length;
    _data[_length] = '\0';
}

CustomString::CustomString(const CustomString &other) : CustomString(other._data) {}

CustomString::~CustomString() {
    delete[] _data;
    _data = nullptr;
    _length = 0U;
    _capacity = 0U;
}

CustomString &CustomString::operator=(const CustomString &other) {
    if (this != &other) {
        ensureCapacity(other._length + 1U);
        copyCharacters(_data, other._data, other._length);
        _length = other._length;
        _data[_length] = '\0';
    }
    return *this;
}

const char *CustomString::cStr() const { return _data ? _data : kEmptyString; }

char *CustomString::data() const { return _data; }

CustomString::SizeType CustomString::length() const { return _length; }

void CustomString::reserve(const SizeType newCapacity) {
    if (newCapacity <= _capacity) return;
    const auto newData = new char[newCapacity];
    if (_data) {
        copyCharacters(newData, _data, _length);
        delete[] _data;
    }
    _data = newData;
    _capacity = newCapacity;
    _data[_length] = '\0';
}

void CustomString::append(const CustomString &other) {
    append(other.cStr());
}

void CustomString::append(const char *text) {
    if (!text) return;
    const SizeType textLength = stringLength(text);
    ensureCapacity(_length + textLength + 1U);
    copyCharacters(_data + _length, text, textLength);
    _length += textLength;
    _data[_length] = '\0';
}

void CustomString::append(const char *text, const SizeType length) {
    if (!text || length == 0U) return;
    ensureCapacity(_length + length + 1U);
    copyCharacters(_data + _length, text, length);
    _length += length;
    _data[_length] = '\0';
}

const char &CustomString::operator[](const SizeType index) const { return _data[index]; }

int CustomString::compare(const CustomString &other) const { return compare(other, CaseSensitivity::Sensitive); }

int CustomString::compare(const CustomString &other, const CaseSensitivity cs) const {
    return compare(other.cStr(), cs);
}

int CustomString::compare(const char *text, const CaseSensitivity cs) const {
    if (!text) text = kEmptyString;
    const char *lhs = cStr();
    const char *rhs = text;
    while (*lhs != '\0' && *rhs != '\0') {
        if (const int diff = charCompare(*lhs, *rhs, cs); diff != 0) return diff;
        ++lhs;
        ++rhs;
    }
    if (*lhs == '\0' && *rhs == '\0') return 0;
    return (*lhs == '\0') ? -1 : 1;
}

CustomString CustomString::trimmed() const {
    if (_length == 0U) return {};
    SizeType start = 0U;
    while (start < _length && isSpace(_data[start])) {
        ++start;
    }
    if (start == _length) return {};
    SizeType end = _length - 1U;
    while (end > start && isSpace(_data[end])) {
        --end;
    }
    const SizeType newLen = end - start + 1U;
    return {_data + start, newLen};
}

CustomString CustomString::toUpper() const {
    CustomString result;
    result.ensureCapacity(_length + 1U);
    for (SizeType i = 0U; i < _length; ++i) {
        result._data[i] = toUpperChar(_data[i]);
    }
    result._length = _length;
    result._data[result._length] = '\0';
    return result;
}

bool CustomString::startsWith(const CustomString &prefix, CaseSensitivity cs) const {
    if (prefix.isEmpty()) return false;
    SizeType index = 0U;
    while (prefix[index] != '\0') {
        if (index >= _length) return false;
        if (charCompare(_data[index], prefix[index], cs) != 0) return false;
        ++index;
    }
    return true;
}

const unsigned char *CustomString::bytes() const {
    return reinterpret_cast<const unsigned char *>(cStr());
}

void CustomString::ensureCapacity(const SizeType needed) {
    if (needed <= _capacity) return;
    SizeType newCapacity = _capacity == 0U ? 1U : _capacity;
    while (newCapacity < needed) newCapacity *= 2U;
    reserve(newCapacity);
}

CustomString::SizeType CustomString::stringLength(const char *text) {
    if (!text) return 0U;
    SizeType count = 0U;
    while (text[count] != '\0') ++count;
    return count;
}

void CustomString::copyCharacters(char *destination, const char *source, const SizeType count) {
    for (SizeType i = 0U; i < count; ++i) destination[i] = source[i];
}

int CustomString::charCompare(char lhs, char rhs, const CaseSensitivity cs) {
    if (cs == CaseSensitivity::Insensitive) {
        lhs = toLowerChar(lhs);
        rhs = toLowerChar(rhs);
    }
    const int diff = static_cast<unsigned char>(lhs) - static_cast<unsigned char>(rhs);
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    return 0;
}

bool CustomString::isSpace(const char ch) {
    return isspace(static_cast<unsigned char>(ch)) != 0;
}

char CustomString::toLowerChar(const char ch) {
    return static_cast<char>(tolower(static_cast<unsigned char>(ch)));
}

char CustomString::toUpperChar(const char ch) {
    return static_cast<char>(toupper(static_cast<unsigned char>(ch)));
}

} // namespace pbl2::core
