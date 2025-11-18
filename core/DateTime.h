#pragma once

#include "core/custom/CustomString.h"

namespace pbl2::core {

class DateTime {
public:
    DateTime() = default;
    DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond = 0);

    bool isValid() const;

    int year() const { return year_; }
    int month() const { return month_; }
    int day() const { return day_; }
    int hour() const { return hour_; }
    int minute() const { return minute_; }
    int second() const { return second_; }
    int millisecond() const { return millisecond_; }

    custom::CustomString toCompactTimestamp() const;
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
