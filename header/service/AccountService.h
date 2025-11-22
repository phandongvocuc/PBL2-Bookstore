#pragma once

#include "core/custom/Optional.h"
#include "core/custom/CustomString.h"
#include "core/custom/DynamicArray.h"

#include "model/Account.h"
#include "StaffService.h"
#include "repository/AccountsRepository.h"
#include "service/BaseService.h"

namespace pbl2::service {

class AccountService final : public BaseService<model::Account, repository::AccountsRepository> {
public:
    explicit AccountService(const custom::CustomString &dataDir);

    [[nodiscard]] custom::DynamicArray<model::Account> fetchAll() const override;
    [[nodiscard]] custom::Optional<model::Account> findByUsername(const custom::CustomString &username) const;
    [[nodiscard]] custom::Optional<model::Account> authenticate(const custom::CustomString &username, const custom::CustomString &password) const;

    [[nodiscard]] bool createAccount(const custom::CustomString &username, const custom::CustomString &password, const custom::CustomString &role, bool active) const;
    [[nodiscard]] bool createAccount(const custom::CustomString &username, const custom::CustomString &password, const custom::CustomString &role, bool active, const custom::CustomString &staffId) const;
    [[nodiscard]] bool updateAccount(const model::Account &account) const;
    [[nodiscard]] bool updatePassword(const custom::CustomString &username, const custom::CustomString &newPassword) const;
    [[nodiscard]] bool removeAccount(const custom::CustomString &username) const;

private:
    StaffService staffService;

    [[nodiscard]] custom::DynamicArray<model::Account> ensureLoaded() const override;
    void persist(const custom::DynamicArray<model::Account> &accounts) const override;
    [[nodiscard]] bool isEmployeeIdInUse(const custom::CustomString &staffId, const custom::CustomString &excludeUsername = custom::CustomString()) const;
    [[nodiscard]] bool createAccountInternal(const custom::CustomString &username, const custom::CustomString &password, const custom::CustomString &role, bool active,
                               const custom::Optional<custom::CustomString> &staffId) const;
};

} // namespace pbl2::service