#pragma once

#include "../../core/Optional.h"
#include "core/Date.h"
#include "../../core/CustomString.h"
#include "../../core/DynamicArray.h"

#include "model/Loan.h"
#include "repository/LoansRepository.h"
#include "service/BaseService.h"

namespace pbl2::service {

    class LoanService final : public BaseService<model::Loan, repository::LoansRepository> {
    public:
        explicit LoanService(const core::CustomString &dataDir);

        [[nodiscard]] core::DynamicArray<model::Loan> fetchAll() const override;
        [[nodiscard]] core::Optional<model::Loan> findById(const core::CustomString &loanId) const override;
        [[nodiscard]] bool createLoan(const model::Loan &loan) const;
        [[nodiscard]] bool updateLoan(const model::Loan &loan) const;
        bool removeLoan(const core::CustomString &loanId) const;
        [[nodiscard]] bool updateStatus(const core::CustomString &loanId, const core::CustomString &status, const core::Date &returnDate = {}) const;
        [[nodiscard]] bool applyFine(const core::CustomString &loanId, int fine) const;
    private:
        [[nodiscard]] core::DynamicArray<model::Loan> ensureLoaded() const override;
        void persist(const core::DynamicArray<model::Loan> &loans) const override;
    };

} // namespace pbl2::service
