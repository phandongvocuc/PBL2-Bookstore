#pragma once

#include "core/custom/CustomString.h"
#include "core/custom/DynamicArray.h"

#include "model/Account.h"

namespace pbl2::repository {

class AccountsRepository {
public:
    explicit AccountsRepository(const custom::CustomString &dataDir);

    [[nodiscard]] custom::DynamicArray<model::Account> loadAll() const;
    void saveAll(const custom::DynamicArray<model::Account> &accounts) const;
    static custom::CustomString hashPassword(const custom::CustomString &plainText);

private:
    custom::CustomString dataPath;
};

}  // namespace pbl2::repository
