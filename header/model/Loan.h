#pragma once
#include "core/Date.h"
#include "../../core/CustomString.h"

namespace pbl2::model {

struct Loan {
    [[nodiscard]] const core::CustomString &getLoanId() const;
    void setLoanId(const core::CustomString &value);

    [[nodiscard]] const core::CustomString &getReaderId() const;
    void setReaderId(const core::CustomString &value);

    [[nodiscard]] const core::CustomString &getBookId() const;
    void setBookId(const core::CustomString &value);

    [[nodiscard]] const core::Date &getBorrowDate() const;
    void setBorrowDate(const core::Date &value);

    [[nodiscard]] const core::Date &getDueDate() const;
    void setDueDate(const core::Date &value);

    [[nodiscard]] const core::Date &getReturnDate() const;
    void setReturnDate(const core::Date &value);

    [[nodiscard]] const core::CustomString &getStatus() const;
    void setStatus(const core::CustomString &value);

    [[nodiscard]] int getFine() const;
    void setFine(int value);

    [[nodiscard]] const core::CustomString &getStaffUsername() const;
    void setStaffUsername(const core::CustomString &value);

private:
    core::CustomString loanId;
    core::CustomString readerId;
    core::CustomString bookId;
    core::Date borrowDate;
    core::Date dueDate;
    core::Date returnDate;
    core::CustomString status;
    int fine{0};
    core::CustomString staffUsername;
};

}  // namespace model
