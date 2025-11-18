#pragma once

#include "core/custom/Optional.h"

#include "core/Date.h"
#include "core/custom/CustomString.h"

#include "core/custom/DynamicArray.h"

#include "model/Loan.h"
#include "repository/LoansRepository.h"

namespace pbl2::service {

class LoanService {
public:
    explicit LoanService(const custom::CustomString &dataDir);

    custom::DynamicArray<model::Loan> fetchAll() const;
    custom::Optional<model::Loan> findById(const custom::CustomString &loanId) const;

    bool createLoan(const model::Loan &loan) const;
    bool updateLoan(const model::Loan &loan) const;
    bool updateStatus(const custom::CustomString &loanId, const custom::CustomString &status, const core::Date &returnDate = {}) const;
    bool applyFine(const custom::CustomString &loanId, int fine) const;
    bool removeLoan(const custom::CustomString &loanId) const;

private:
    repository::LoansRepository repository;

    custom::DynamicArray<model::Loan> ensureLoaded() const;
    void persist(const custom::DynamicArray<model::Loan> &loans) const;
};

}  // namespace pbl2::service
