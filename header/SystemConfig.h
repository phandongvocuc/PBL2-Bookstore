#pragma once

namespace pbl2::model {

class SystemConfig {
public:
    [[nodiscard]] int getMaxBorrowDays() const { return maxBorrowDays; }
    void setMaxBorrowDays(const int value) { maxBorrowDays = value; }

    [[nodiscard]] int getFinePerDay() const { return finePerDay; }
    void setFinePerDay(const int value) { finePerDay = value; }

    [[nodiscard]] int getMaxBooksPerReader() const { return maxBooksPerReader; }
    void setMaxBooksPerReader(const int value) { maxBooksPerReader = value; }

private:
    int maxBorrowDays{0};
    int finePerDay{0};
    int maxBooksPerReader{0};
};

}  // namespace model


