#include "service/AccountService.h"
#include "core/DateTime.h"
#include "core/IdGenerator.h"

namespace pbl2::service {

AccountService::AccountService(const custom::CustomString &dataDir)
    : BaseService(dataDir), staffService(dataDir) {}

custom::DynamicArray<model::Account> AccountService::fetchAll() const {
    return BaseService::fetchAll();
}

custom::Optional<model::Account> AccountService::findByUsername(const custom::CustomString &username) const {
    return BaseService::findById(username);
}

custom::Optional<model::Account> AccountService::authenticate(const custom::CustomString &username, const custom::CustomString &password) const {
    const custom::CustomString trimmed = username.trimmed();
    if (trimmed.isEmpty() || password.isEmpty()) return {};

    const auto accounts = ensureLoaded();
    if (accounts.isEmpty()) return {};

    const custom::CustomString hashed = repository::AccountsRepository::hashPassword(password);
    model::Account *candidate = nullptr;

    for (auto &account : accounts) {
        if (account.getUsername().compare(trimmed, custom::CaseSensitivity::Insensitive) == 0) {
            candidate = &account;
            break;
        }
    }

    if (!candidate) return {};
    if (candidate->getPasswordHash() != hashed) return {};
    if (!candidate->isActive()) return {};

    candidate->setLastLogin(core::DateTime::nowUtc());
    persist(accounts);
    return custom::Optional(*candidate);
}

bool AccountService::createAccount(const custom::CustomString &username, const custom::CustomString &password, const custom::CustomString &role, bool active) const {
    const custom::Optional<custom::CustomString> none;
    return createAccountInternal(username, password, role, active, none);
}

bool AccountService::createAccount(const custom::CustomString &username, const custom::CustomString &password, const custom::CustomString &role, bool active,
                                   const custom::CustomString &staffId) const {
    const custom::CustomString cleanStaffId = staffId.trimmed();
    if (!cleanStaffId.isEmpty()) {
        if (!staffService.findById(cleanStaffId).has_value()) return false;
        if (isEmployeeIdInUse(cleanStaffId)) return false;
    }
    const custom::Optional<custom::CustomString> staffOpt =
        cleanStaffId.isEmpty() ? custom::Optional<custom::CustomString>() : custom::Optional(cleanStaffId);
    return createAccountInternal(username, password, role, active, staffOpt);
}

bool AccountService::updateAccount(const model::Account &account) const {
    if (const custom::CustomString newStaffId = account.getEmployeeId().trimmed(); !newStaffId.isEmpty()) {
        if (!staffService.findById(newStaffId).has_value()) return false;
        if (isEmployeeIdInUse(newStaffId, account.getUsername())) return false;
    }
    return updateItem(account);
}

bool AccountService::updatePassword(const custom::CustomString &username, const custom::CustomString &newPassword) const {
    const custom::CustomString trimmed = username.trimmed();
    if (trimmed.isEmpty() || newPassword.isEmpty()) return false;

    auto accounts = ensureLoaded();
    bool updated = false;
    for (auto &account : accounts) {
        if (account.getUsername().compare(trimmed, custom::CaseSensitivity::Insensitive) == 0) {
            account.setPasswordHash(repository::AccountsRepository::hashPassword(newPassword));
            updated = true;
            break;
        }
    }
    if (!updated) return false;
    persist(accounts);
    return true;
}

bool AccountService::removeAccount(const custom::CustomString &username) const {
    return removeItem(username);
}

custom::DynamicArray<model::Account> AccountService::ensureLoaded() const {
    auto accounts = repository.loadAll();
    custom::DynamicArray<custom::CustomString> ids;
    ids.reserve(accounts.size());
    bool mutated = false;
    for (auto &acc : accounts) {
        custom::CustomString id = acc.getAccountId().trimmed();
        if (id.isEmpty()) {
            id = core::IdGenerator::nextId(ids, custom::CustomStringLiteral("AC"), 3);
            acc.setAccountId(id);
            mutated = true;
        }
        ids.pushBack(id);
    }
    if (mutated) {
        persist(accounts);
    }
    return accounts;
}

void AccountService::persist(const custom::DynamicArray<model::Account> &accounts) const {
    repository.saveAll(accounts);
}

bool AccountService::isEmployeeIdInUse(const custom::CustomString &staffId, const custom::CustomString &excludeUsername) const {
    const custom::CustomString trimmedStaff = staffId.trimmed();
    if (trimmedStaff.isEmpty()) return false;
    const custom::CustomString trimmedExclude = excludeUsername.trimmed();

    for (const auto accounts = ensureLoaded(); const auto &acc : accounts) {
        if (acc.getEmployeeId().trimmed().isEmpty()) continue;
        if (acc.getEmployeeId().compare(trimmedStaff, custom::CaseSensitivity::Insensitive) != 0) continue;
        if (!trimmedExclude.isEmpty() && acc.getUsername().compare(trimmedExclude, custom::CaseSensitivity::Insensitive) == 0) {
            continue;
        }
        return true;
    }
    return false;
}

bool AccountService::createAccountInternal(const custom::CustomString &username, const custom::CustomString &password, const custom::CustomString &role, const bool active,
                                           const custom::Optional<custom::CustomString> &staffId) const {
    const custom::CustomString cleanUsername = username.trimmed();
    const custom::CustomString cleanRole = role.trimmed();
    const custom::CustomString employee = staffId.has_value() ? staffId.value().trimmed() : custom::CustomString();
    if (cleanUsername.isEmpty() || password.isEmpty()) return false;

    // Check if username exists using BaseService
    if (findById(cleanUsername).has_value()) {
        return false;
    }

    if (!employee.isEmpty() && isEmployeeIdInUse(employee)) {
        return false;
    }

    const auto accounts = ensureLoaded();
    custom::DynamicArray<custom::CustomString> ids;
    ids.reserve(accounts.size());
    for (const auto &account : accounts) {
        if (const custom::CustomString existingId = account.getAccountId().trimmed(); !existingId.isEmpty()) {
            ids.pushBack(existingId);
        }
    }

    model::Account account;
    account.setAccountId(core::IdGenerator::nextId(ids, custom::CustomStringLiteral("AC"), 3));
    account.setUsername(cleanUsername);
    account.setPasswordHash(repository::AccountsRepository::hashPassword(password));
    account.setRole(cleanRole.isEmpty() ? custom::CustomStringLiteral("STAFF") : cleanRole);
    account.setActive(active);
    account.setCreatedAt(core::DateTime::nowUtc());
    account.setLastLogin(core::DateTime());
    if (!employee.isEmpty()) account.setEmployeeId(employee);

    return BaseService::addItem(account);
}

} // namespace pbl2::service