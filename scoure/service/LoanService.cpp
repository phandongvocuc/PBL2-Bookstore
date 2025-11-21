#include "service/LoanService.h"

using namespace std;

namespace pbl2::service {

LoanService::LoanService(const custom::CustomString &dataDir) : repository(dataDir) {}

custom::DynamicArray<model::Loan> LoanService::fetchAll() const { return ensureLoaded(); }

custom::Optional<model::Loan> LoanService::findById(const custom::CustomString &loanId) const {
    const auto trimmed = loanId.trimmed();
    if (trimmed.isEmpty()) return {};
    const auto loans = ensureLoaded();
    if (loans.isEmpty()) return {};
    for (const auto & loan : loans) {
        if (loan.getLoanId().compare(trimmed, custom::CaseSensitivity::Insensitive) == 0) {
            return custom::Optional(loan);
        }
    }
    return {};
}

bool LoanService::createLoan(const model::Loan &loan) const {
    if (!loan.getBorrowDate().isValid() || !loan.getDueDate().isValid()) return false;
    auto loans = ensureLoaded();

    const model::Loan& copy = loan;
    bool exists = false;
    for (const auto & loan1 : loans) {
        if (loan1.getLoanId().compare(copy.getLoanId(), custom::CaseSensitivity::Insensitive) == 0) {
            exists = true;
            break;
        }
    }
    if (exists) return false;

    loans.pushBack(copy);
    persist(loans);
    return true;
}

bool LoanService::updateLoan(const model::Loan &loan) const {
    const auto loans = ensureLoaded();
    bool updated = false;
    for (auto & it : loans) {
        if (it.getLoanId().compare(loan.getLoanId(), custom::CaseSensitivity::Insensitive) == 0) {
            it = loan;
            updated = true;
            break;
        }
    }
    if (!updated) return false;
    persist(loans);
    return true;
}

bool LoanService::updateStatus(const custom::CustomString &loanId, const custom::CustomString &status, const core::Date &returnDate) const {
    const auto loans = ensureLoaded();
    bool changed = false;
    for (auto & loan : loans) {
        if (loan.getLoanId().compare(loanId, custom::CaseSensitivity::Insensitive) == 0) {
            if (const auto normalizedStatus = status.trimmed(); !normalizedStatus.isEmpty()) {
                loan.setStatus(normalizedStatus.toUpper());
            }
            if (returnDate.isValid()) loan.setReturnDate(returnDate);
            changed = true;
            break;
        }
    }
    if (!changed) return false;
    persist(loans);
    return true;
}

bool LoanService::applyFine(const custom::CustomString &loanId, const int fine) const {
    if (fine < 0) return false;
    const auto loans = ensureLoaded();
    bool updated = false;
    for (auto & loan : loans) {
        if (loan.getLoanId().compare(loanId, custom::CaseSensitivity::Insensitive) == 0) {
            loan.setFine(fine);
            updated = true;
            break;
        }
    }
    if (!updated) return false;
    persist(loans);
    return true;
}

bool LoanService::removeLoan(const custom::CustomString &loanId) const {
    auto loans = ensureLoaded();
    bool removed = false;
    custom::DynamicArray<model::Loan>::SizeType index = 0U;
    while (index < loans.size()) {
        if (loans[index].getLoanId().compare(loanId, custom::CaseSensitivity::Insensitive) == 0) {
            loans.removeAt(index);
            removed = true;
        } else {
            ++index;
        }
    }
    if (!removed) return false;
    persist(loans);
    return true;
}

custom::DynamicArray<model::Loan> LoanService::ensureLoaded() const {
    return repository.loadAll();
}

void LoanService::persist(const custom::DynamicArray<model::Loan> &loans) const {
    repository.saveAll(loans);
}

}  // namespace pbl2::service
