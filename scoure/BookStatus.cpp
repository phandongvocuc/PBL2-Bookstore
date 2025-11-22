#include "BookStatus.h"

namespace pbl2::model {

namespace {

bool equals(const core::CustomString &value, const char *literal) {
    return value.trimmed().compare(literal, core::CaseSensitivity::Insensitive) == 0;
}

core::CustomString normalizedUpper(const core::CustomString &status) {
    return status.trimmed().toUpper();
}

}  // namespace

core::CustomString canonicalBookStatus(const core::CustomString &status) {
    const core::CustomString upper = normalizedUpper(status);
    if (equals(upper, "AVAILABLE") || equals(upper, "CÒN") || equals(upper, "CON")) {
        return core::CustomStringLiteral("CÒN");
    }
    if (equals(upper, "OUT") || equals(upper, "OUT_OF_STOCK") || equals(upper, "BORROWED") || equals(upper, "HẾT") || equals(upper, "HET")) {
        return core::CustomStringLiteral("HẾT");
    }
    if (equals(upper, "LOST") || equals(upper, "MẤT") || equals(upper, "MAT")) {
        return core::CustomStringLiteral("MẤT");
    }
    if (equals(upper, "DAMAGED") || equals(upper, "HƯ") || equals(upper, "HỎNG") || equals(upper, "HU") || equals(upper, "HONG")) {
        return core::CustomStringLiteral("HỎNG");
    }
    if (upper.isEmpty()) {
        return core::CustomStringLiteral("CÒN");
    }
    return upper;
}

core::CustomString availabilityStatusForQuantity(const int quantity) {
    return quantity > 0 ? core::CustomStringLiteral("CÒN") : core::CustomStringLiteral("HẾT");
}

bool isAvailabilityStatus(const core::CustomString &status) {
    const core::CustomString canonical = canonicalBookStatus(status);
    return canonical.compare(core::CustomStringLiteral("CÒN")) == 0 ||
           canonical.compare(core::CustomStringLiteral("HẾT")) == 0;
}

}  // namespace pbl2::model
