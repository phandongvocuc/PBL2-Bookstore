#include "model/Loan.h"

namespace pbl2::model {

const core::CustomString &Loan::getLoanId() const {
    return loanId;
}

void Loan::setLoanId(const core::CustomString &value) {
    loanId = value;
}

const core::CustomString &Loan::getReaderId() const {
    return readerId;
}

void Loan::setReaderId(const core::CustomString &value) {
    readerId = value;
}

const core::CustomString &Loan::getBookId() const {
    return bookId;
}

void Loan::setBookId(const core::CustomString &value) {
    bookId = value;
}

const core::Date &Loan::getBorrowDate() const {
    return borrowDate;
}

void Loan::setBorrowDate(const core::Date &value) {
    borrowDate = value;
}

const core::Date &Loan::getDueDate() const {
    return dueDate;
}

void Loan::setDueDate(const core::Date &value) {
    dueDate = value;
}

const core::Date &Loan::getReturnDate() const {
    return returnDate;
}

void Loan::setReturnDate(const core::Date &value) {
    returnDate = value;
}

const core::CustomString &Loan::getStatus() const {
    return status;
}

void Loan::setStatus(const core::CustomString &value) {
    status = value;
}

int Loan::getFine() const {
    return fine;
}

void Loan::setFine(const int value) {
    fine = value;
}

const core::CustomString &Loan::getStaffUsername() const {
    return staffUsername;
}

void Loan::setStaffUsername(const core::CustomString &value) {
    staffUsername = value;
}

}  // namespace pbl2::model
