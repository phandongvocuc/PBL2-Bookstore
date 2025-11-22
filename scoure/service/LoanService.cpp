#include "service/LoanService.h"

namespace pbl2::service {

LoanService::LoanService(const custom::CustomString &dataDir)
    : BaseService(dataDir) {}

custom::DynamicArray<model::Loan> LoanService::fetchAll() const {
    return BaseService::fetchAll();
}

custom::Optional<model::Loan> LoanService::findById(const custom::CustomString &loanId) const {
    return BaseService::findById(loanId);
}

bool LoanService::createLoan(const model::Loan &loan) const {
    if (!loan.getBorrowDate().isValid() || !loan.getDueDate().isValid()) return false;
    return addItem(loan);
}

bool LoanService::updateLoan(const model::Loan &loan) const {
    return updateItem(loan);
}

bool LoanService::removeLoan(const custom::CustomString &loanId) const {
    return removeItem(loanId);
}

bool LoanService::updateStatus(const custom::CustomString &loanId, const custom::CustomString &status, const core::Date &returnDate) const {
    const auto loans = ensureLoaded();
    bool changed = false;
    for (auto &loan : loans) {
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
    for (auto &loan : loans) {
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
    custom::DynamicArray<model::Loan> LoanService::ensureLoaded() const {
        return repository.loadAll();
    }

    void LoanService::persist(const custom::DynamicArray<model::Loan> &loans) const {
        repository.saveAll(loans);
}

} // namespace pbl2::service