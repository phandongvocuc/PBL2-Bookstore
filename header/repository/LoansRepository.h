#pragma once

#include "core/custom/CustomString.h"

#include "core/custom/DynamicArray.h"

#include "model/Loan.h"

namespace pbl2::repository {

class LoansRepository {
public:
    explicit LoansRepository(const custom::CustomString &dataDir);

    custom::DynamicArray<model::Loan> loadAll() const;
    void saveAll(const custom::DynamicArray<model::Loan> &loans) const;

private:
    custom::CustomString dataPath;
};

}  // namespace pbl2::repository
