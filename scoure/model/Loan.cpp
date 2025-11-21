#include "model/Loan.h"

using namespace std;  // project-wide request

namespace pbl2::model {

const custom::CustomString &Loan::getLoanId() const {
    return loanId;
}

void Loan::setLoanId(const custom::CustomString &value) {
    loanId = value;
}

const custom::CustomString &Loan::getReaderId() const {
    return readerId;
}

void Loan::setReaderId(const custom::CustomString &value) {
    readerId = value;
}

const custom::CustomString &Loan::getBookId() const {
    return bookId;
}

void Loan::setBookId(const custom::CustomString &value) {
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

const custom::CustomString &Loan::getStatus() const {
    return status;
}

void Loan::setStatus(const custom::CustomString &value) {
    status = value;
}

int Loan::getFine() const {
    return fine;
}

void Loan::setFine(int value) {
    fine = value;
}

const custom::CustomString &Loan::getStaffUsername() const {
    return staffUsername;
}

void Loan::setStaffUsername(const custom::CustomString &value) {
    staffUsername = value;
}

}  // namespace pbl2::model
