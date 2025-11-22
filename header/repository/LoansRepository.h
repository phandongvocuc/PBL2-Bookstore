#pragma once

#include "../../core/CustomString.h"
#include "../../core/DynamicArray.h"

#include "model/Loan.h"

namespace pbl2::repository {

class LoansRepository {
public:
    explicit LoansRepository(const core::CustomString &dataDir);

    [[nodiscard]] core::DynamicArray<model::Loan> loadAll() const;
    void saveAll(const core::DynamicArray<model::Loan> &loans) const;

private:
    core::CustomString dataPath;
};

}  // namespace pbl2::repository
