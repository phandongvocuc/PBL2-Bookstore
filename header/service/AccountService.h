#pragma once

#include "../../core/Optional.h"
#include "../../core/CustomString.h"
#include "../../core/DynamicArray.h"

#include "model/Account.h"
#include "StaffService.h"
#include "repository/AccountsRepository.h"
#include "service/BaseService.h"

namespace pbl2::service {

class AccountService final : public BaseService<model::Account, repository::AccountsRepository> {
public:
    explicit AccountService(const core::CustomString &dataDir);

    [[nodiscard]] core::DynamicArray<model::Account> fetchAll() const override;
    [[nodiscard]] core::Optional<model::Account> findByUsername(const core::CustomString &username) const;
    [[nodiscard]] core::Optional<model::Account> authenticate(const core::CustomString &username, const core::CustomString &password) const;

    [[nodiscard]] bool createAccount(const core::CustomString &username, const core::CustomString &password, const core::CustomString &role, bool active) const;
    [[nodiscard]] bool createAccount(const core::CustomString &username, const core::CustomString &password, const core::CustomString &role, bool active, const core::CustomString &staffId) const;
    [[nodiscard]] bool updateAccount(const model::Account &account) const;
    [[nodiscard]] bool updatePassword(const core::CustomString &username, const core::CustomString &newPassword) const;
    [[nodiscard]] bool removeAccount(const core::CustomString &username) const;

private:
    StaffService staffService;

    [[nodiscard]] core::DynamicArray<model::Account> ensureLoaded() const override;
    void persist(const core::DynamicArray<model::Account> &accounts) const override;
    [[nodiscard]] bool isEmployeeIdInUse(const core::CustomString &staffId, const core::CustomString &excludeUsername = core::CustomString()) const;
    [[nodiscard]] bool createAccountInternal(const core::CustomString &username, const core::CustomString &password, const core::CustomString &role, bool active,
                               const core::Optional<core::CustomString> &staffId) const;
};

} // namespace pbl2::service
