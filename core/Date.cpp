#include "core/Date.h"

namespace pbl2::core {

Date::Date(const int year, const int month, const int day) : year_(year), month_(month), day_(day) {}

bool Date::isValid() const {
    if (year_ <= 0 || month_ <= 0 || day_ <= 0) return false;
    if (month_ > 12) return false;
    const int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int maxDay = daysInMonth[month_ - 1];
    if (const bool leap = (year_ % 4 == 0 && (year_ % 100 != 0 || year_ % 400 == 0)); month_ == 2 && leap) {
        maxDay = 29;
    }
    return day_ <= maxDay;
}

}  // namespace pbl2::core

