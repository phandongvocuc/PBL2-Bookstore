#pragma once

namespace pbl2::core{

class Date {
public:
    Date() = default;
    Date(int year, int month, int day);

    [[nodiscard]] int year() const { return year_; }
    [[nodiscard]] int month() const { return month_; }
    [[nodiscard]] int day() const { return day_; }

    [[nodiscard]] bool isValid() const;

private:
    int year_{0};
    int month_{0};
    int day_{0};
};

}  // namespace core


