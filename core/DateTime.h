#pragma once

#include "CustomString.h"

namespace pbl2::core {

class DateTime {
public:
    DateTime() = default;
    DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond = 0);

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] int year() const { return year_; }
    [[nodiscard]] int month() const { return month_; }
    [[nodiscard]] int day() const { return day_; }
    [[nodiscard]] int hour() const { return hour_; }
    [[nodiscard]] int minute() const { return minute_; }
    [[nodiscard]] int second() const { return second_; }
    [[nodiscard]] int millisecond() const { return millisecond_; }

    [[nodiscard]] core::CustomString toCompactTimestamp() const;
    static DateTime nowUtc();

private:
    int year_{0};
    int month_{0};
    int day_{0};
    int hour_{0};
    int minute_{0};
    int second_{0};
    int millisecond_{0};
};

}  // namespace pbl2::core
