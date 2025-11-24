#include "service/LoanService.h"

namespace pbl2::service {

LoanService::LoanService(const core::CustomString &dataDir)
    : BaseService(dataDir) {}

core::DynamicArray<model::Loan> LoanService::fetchAll() const {
    return BaseService::fetchAll();
}

core::Optional<model::Loan> LoanService::findById(const core::CustomString &loanId) const {
    return BaseService::findById(loanId);
}

bool LoanService::createLoan(const model::Loan &loan) const {
    if (!loan.getBorrowDate().isValid() || !loan.getDueDate().isValid()) return false;
    return addItem(loan);
}

bool LoanService::updateLoan(const model::Loan &loan) const {
    return updateItem(loan);
}

bool LoanService::removeLoan(const core::CustomString &loanId) const {
    return removeItem(loanId);
}

bool LoanService::updateStatus(const core::CustomString &loanId, const core::CustomString &status, const core::Date &returnDate) const {
    auto loans = ensureLoaded();
    bool changed = false;
    for (auto &loan : loans) {
        if (loan.getLoanId().compare(loanId, core::CaseSensitivity::Insensitive) == 0) {
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

bool LoanService::applyFine(const core::CustomString &loanId, const int fine) const {
    if (fine < 0) return false;
    auto loans = ensureLoaded();
    bool updated = false;
    for (auto &loan : loans) {
        if (loan.getLoanId().compare(loanId, core::CaseSensitivity::Insensitive) == 0) {
            loan.setFine(fine);
            updated = true;
            break;
        }
    }
    if (!updated) return false;
    persist(loans);
    return true;
}
    core::DynamicArray<model::Loan> LoanService::ensureLoaded() const {
        return repository.loadAll();
    }

    void LoanService::persist(const core::DynamicArray<model::Loan> &loans) const {
        repository.saveAll(loans);
}

} // namespace pbl2::service
