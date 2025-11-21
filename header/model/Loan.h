#pragma once
#include "core/Date.h"
#include "core/custom/CustomString.h"

namespace pbl2::model {

struct Loan {
    [[nodiscard]] const custom::CustomString &getLoanId() const;
    void setLoanId(const custom::CustomString &value);

    [[nodiscard]] const custom::CustomString &getReaderId() const;
    void setReaderId(const custom::CustomString &value);

    [[nodiscard]] const custom::CustomString &getBookId() const;
    void setBookId(const custom::CustomString &value);

    [[nodiscard]] const core::Date &getBorrowDate() const;
    void setBorrowDate(const core::Date &value);

    [[nodiscard]] const core::Date &getDueDate() const;
    void setDueDate(const core::Date &value);

    [[nodiscard]] const core::Date &getReturnDate() const;
    void setReturnDate(const core::Date &value);

    [[nodiscard]] const custom::CustomString &getStatus() const;
    void setStatus(const custom::CustomString &value);

    [[nodiscard]] int getFine() const;
    void setFine(int value);

    [[nodiscard]] const custom::CustomString &getStaffUsername() const;
    void setStaffUsername(const custom::CustomString &value);

private:
    custom::CustomString loanId;
    custom::CustomString readerId;
    custom::CustomString bookId;
    core::Date borrowDate;
    core::Date dueDate;
    core::Date returnDate;
    custom::CustomString status;
    int fine{0};
    custom::CustomString staffUsername;
};

}  // namespace model
