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
    if (equals(upper, "AVAILABLE") || equals(upper, "CON")) {
        return custom::CustomStringLiteral("CON");
    }
    if (equals(upper, "OUT") || equals(upper, "OUT_OF_STOCK") || equals(upper, "BORROWED") || equals(upper, "HET")) {
        return custom::CustomStringLiteral("HET");
    }
    if (equals(upper, "LOST") || equals(upper, "MAT")) {
        return custom::CustomStringLiteral("MAT");
    }
    if (equals(upper, "DAMAGED") || equals(upper, "HU") || equals(upper, "HONG")) {
        return custom::CustomStringLiteral("HONG");
    }
    if (upper.isEmpty()) {
        return custom::CustomStringLiteral("CON");
    }
    return upper;
}

custom::CustomString availabilityStatusForQuantity(const int quantity) {
    return quantity > 0 ? custom::CustomStringLiteral("CON") : custom::CustomStringLiteral("HET");
}

bool isAvailabilityStatus(const custom::CustomString &status) {
    const custom::CustomString canonical = canonicalBookStatus(status);
    return canonical.compare(custom::CustomStringLiteral("CON")) == 0 ||
           canonical.compare(custom::CustomStringLiteral("HET")) == 0;
}

}  // namespace pbl2::model
