#pragma once

namespace pbl2::core{

class Date {
public:
    Date() = default;
    Date(int year, int month, int day);

    int year() const { return year_; }
    int month() const { return month_; }
    int day() const { return day_; }

    bool isValid() const;

private:
    int year_{0};
    int month_{0};
    int day_{0};
};

}  // namespace core


