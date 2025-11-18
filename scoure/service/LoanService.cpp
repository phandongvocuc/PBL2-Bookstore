#include "service/LoanService.h"

using namespace std;

namespace pbl2::service {

LoanService::LoanService(const custom::CustomString &dataDir) : repository(dataDir) {}

custom::DynamicArray<model::Loan> LoanService::fetchAll() const { return ensureLoaded(); }

custom::Optional<model::Loan> LoanService::findById(const custom::CustomString &loanId) const {
    const auto trimmed = loanId.trimmed();
    if (trimmed.isEmpty()) return custom::Optional<model::Loan>();
    const auto loans = ensureLoaded();
    if (loans.isEmpty()) return custom::Optional<model::Loan>();
    for (custom::DynamicArray<model::Loan>::ConstIterator it = loans.cbegin(); it != loans.cend(); ++it) {
        if (it->getLoanId().compare(trimmed, custom::CaseSensitivity::Insensitive) == 0) {
            return custom::Optional(*it);
        }
    }
    return custom::Optional<model::Loan>();
}

bool LoanService::createLoan(const model::Loan &loan) const {
    if (!loan.getBorrowDate().isValid() || !loan.getDueDate().isValid()) return false;
    auto loans = ensureLoaded();

    const model::Loan copy = loan;
    bool exists = false;
    for (custom::DynamicArray<model::Loan>::ConstIterator it = loans.cbegin(); it != loans.cend(); ++it) {
        if (it->getLoanId().compare(copy.getLoanId(), custom::CaseSensitivity::Insensitive) == 0) {
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
    auto loans = ensureLoaded();
    bool updated = false;
    for (custom::DynamicArray<model::Loan>::Iterator it = loans.begin(); it != loans.end(); ++it) {
        if (it->getLoanId().compare(loan.getLoanId(), custom::CaseSensitivity::Insensitive) == 0) {
            *it = loan;
            updated = true;
            break;
        }
    }
    if (!updated) return false;
    persist(loans);
    return true;
}

bool LoanService::updateStatus(const custom::CustomString &loanId, const custom::CustomString &status, const core::Date &returnDate) const {
    auto loans = ensureLoaded();
    bool changed = false;
    for (custom::DynamicArray<model::Loan>::Iterator it = loans.begin(); it != loans.end(); ++it) {
        if (it->getLoanId().compare(loanId, custom::CaseSensitivity::Insensitive) == 0) {
            if (const auto normalizedStatus = status.trimmed(); !normalizedStatus.isEmpty()) {
                it->setStatus(normalizedStatus.toUpper());
            }
            if (returnDate.isValid()) it->setReturnDate(returnDate);
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
    auto loans = ensureLoaded();
    bool updated = false;
    for (custom::DynamicArray<model::Loan>::Iterator it = loans.begin(); it != loans.end(); ++it) {
        if (it->getLoanId().compare(loanId, custom::CaseSensitivity::Insensitive) == 0) {
            it->setFine(fine);
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
