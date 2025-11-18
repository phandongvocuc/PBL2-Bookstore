#pragma once

#include "core/custom/Optional.h"

#include "core/custom/CustomString.h"

#include "core/custom/DynamicArray.h"

#include "model/Account.h"
#include "StaffService.h"
#include "repository/AccountsRepository.h"

namespace pbl2::service {

class AccountService {
public:
    explicit AccountService(const custom::CustomString &dataDir);

    custom::DynamicArray<model::Account> fetchAll() const;
    custom::Optional<model::Account> findByUsername(const custom::CustomString &username) const;
    custom::Optional<model::Account> authenticate(const custom::CustomString &username, const custom::CustomString &password) const;

    bool createAccount(const custom::CustomString &username, const custom::CustomString &password, const custom::CustomString &role, bool active) const;
    // Overload: create account and bind to a staff by staffId (must exist and be unique)
    bool createAccount(const custom::CustomString &username, const custom::CustomString &password, const custom::CustomString &role, bool active, const custom::CustomString &staffId) const;
    bool updateAccount(const model::Account &account) const;
    bool updatePassword(const custom::CustomString &username, const custom::CustomString &newPassword) const;
    bool removeAccount(const custom::CustomString &username) const;

private:
    repository::AccountsRepository repository;
    StaffService staffService;

    custom::DynamicArray<model::Account> ensureLoaded() const;
    void persist(const custom::DynamicArray<model::Account> &accounts) const;
    bool isEmployeeIdInUse(const custom::CustomString &staffId, const custom::CustomString &excludeUsername = custom::CustomString()) const;
    bool createAccountInternal(const custom::CustomString &username, const custom::CustomString &password, const custom::CustomString &role, bool active,
                               const custom::Optional<custom::CustomString> &staffId) const;
};

}  // namespace pbl2::service
