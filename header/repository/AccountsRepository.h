#pragma once

#include "../../core/CustomString.h"
#include "../../core/DynamicArray.h"

#include "model/Account.h"

namespace pbl2::repository {

class AccountsRepository {
public:
    explicit AccountsRepository(const core::CustomString &dataDir);

    [[nodiscard]] core::DynamicArray<model::Account> loadAll() const;
    void saveAll(const core::DynamicArray<model::Account> &accounts) const;
    static core::CustomString hashPassword(const core::CustomString &plainText);

private:
    core::CustomString dataPath;
};

}  // namespace pbl2::repository
