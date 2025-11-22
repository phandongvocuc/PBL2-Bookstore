#include "BookStatus.h"

namespace pbl2::model {

namespace {

bool equals(const custom::CustomString &value, const char *literal) {
    return value.trimmed().compare(literal, custom::CaseSensitivity::Insensitive) == 0;
}

custom::CustomString normalizedUpper(const custom::CustomString &status) {
    return status.trimmed().toUpper();
}

}  // namespace

custom::CustomString canonicalBookStatus(const custom::CustomString &status) {
    const custom::CustomString upper = normalizedUpper(status);
    if (equals(upper, "AVAILABLE") || equals(upper, "CÒN") || equals(upper, "CON")) {
        return custom::CustomStringLiteral("CÒN");
    }
    if (equals(upper, "OUT") || equals(upper, "OUT_OF_STOCK") || equals(upper, "BORROWED") || equals(upper, "HẾT") || equals(upper, "HET")) {
        return custom::CustomStringLiteral("HẾT");
    }
    if (equals(upper, "LOST") || equals(upper, "MẤT") || equals(upper, "MAT")) {
        return custom::CustomStringLiteral("MẤT");
    }
    if (equals(upper, "DAMAGED") || equals(upper, "HƯ") || equals(upper, "HỎNG") || equals(upper, "HU") || equals(upper, "HONG")) {
        return custom::CustomStringLiteral("HỎNG");
    }
    if (upper.isEmpty()) {
        return custom::CustomStringLiteral("CÒN");
    }
    return upper;
}

custom::CustomString availabilityStatusForQuantity(const int quantity) {
    return quantity > 0 ? custom::CustomStringLiteral("CÒN") : custom::CustomStringLiteral("HẾT");
}

bool isAvailabilityStatus(const custom::CustomString &status) {
    const custom::CustomString canonical = canonicalBookStatus(status);
    return canonical.compare(custom::CustomStringLiteral("CÒN")) == 0 ||
           canonical.compare(custom::CustomStringLiteral("HẾT")) == 0;
}

}  // namespace pbl2::model
