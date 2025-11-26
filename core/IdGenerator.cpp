#include "core/IdGenerator.h"
#include "core/DynamicArray.h"
#include "core/CustomString.h"
#include <string>

using namespace std;

namespace pbl2::core {

static int extractTrailingNumberCustom(const core::CustomString &value, const core::CustomString &prefix) {
    if (!value.startsWith(prefix, core::CaseSensitivity::Insensitive)) return -1;
    const char *data = value.cStr();
    const auto total = value.length();
    const auto prefixLen = prefix.length();
    if (total <= prefixLen) return -1;
    int result = 0;
    bool hasDigit = false;
    for (core::CustomString::SizeType i = prefixLen; i < total; ++i) {
        const auto ch = static_cast<unsigned char>(data[i]);
        if (!std::isdigit(ch)) return -1;
        hasDigit = true;
        result = result * 10 + (ch - '0');
    }
    return hasDigit ? result : -1;
}

static core::CustomString buildIdentifier(const core::CustomString &prefix, const int value, const int width) {
    string digits = to_string(value);
    if (width > 0 && static_cast<int>(digits.size()) < width) {
        digits.insert(digits.begin(), width - digits.size(), '0');
    }
    core::CustomString result(prefix.trimmed());
    result.append(digits.c_str(), static_cast<core::CustomString::SizeType>(digits.size()));
    return result;
}

core::CustomString IdGenerator::nextId(const core::DynamicArray<core::CustomString> &existing,
                                         const core::CustomString &prefix,
                                         const int width) {
    int maxVal = 0;
    for (core::DynamicArray<core::CustomString>::SizeType i = 0U; i < existing.size(); ++i) {
        if (const int v = extractTrailingNumberCustom(existing[i].trimmed(), prefix); v > maxVal) maxVal = v;
    }
    const int next = maxVal + 1;
    return buildIdentifier(prefix, next, width);
}

}  // namespace pbl2::core
