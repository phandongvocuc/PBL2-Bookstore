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

    [[nodiscard]] custom::DynamicArray<model::Loan> fetchAll() const;
    [[nodiscard]] custom::Optional<model::Loan> findById(const custom::CustomString &loanId) const;

    [[nodiscard]] bool createLoan(const model::Loan &loan) const;
    [[nodiscard]] bool updateLoan(const model::Loan &loan) const;
    [[nodiscard]] bool updateStatus(const custom::CustomString &loanId, const custom::CustomString &status, const core::Date &returnDate = {}) const;
    [[nodiscard]] bool applyFine(const custom::CustomString &loanId, int fine) const;
    bool removeLoan(const custom::CustomString &loanId) const;

private:
    repository::LoansRepository repository;

    [[nodiscard]] custom::DynamicArray<model::Loan> ensureLoaded() const;
    void persist(const custom::DynamicArray<model::Loan> &loans) const;
};

}  // namespace pbl2::service
