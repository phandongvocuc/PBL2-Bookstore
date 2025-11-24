#include "service/AccountService.h"
#include "core/DateTime.h"
#include "core/IdGenerator.h"

namespace pbl2::service {

AccountService::AccountService(const core::CustomString &dataDir)
    : BaseService(dataDir), staffService(dataDir) {}

core::DynamicArray<model::Account> AccountService::fetchAll() const {
    return BaseService::fetchAll();
}

core::Optional<model::Account> AccountService::findByUsername(const core::CustomString &username) const {
    return BaseService::findById(username);
}

core::Optional<model::Account> AccountService::authenticate(const core::CustomString &username, const core::CustomString &password) const {
    const core::CustomString trimmed = username.trimmed();
    if (trimmed.isEmpty() || password.isEmpty()) return {};

    auto accounts = ensureLoaded();
    if (accounts.isEmpty()) return {};

    const core::CustomString hashed = repository::AccountsRepository::hashPassword(password);
    model::Account *candidate = nullptr;

    for (auto &account : accounts) {
        if (account.getUsername().compare(trimmed, core::CaseSensitivity::Insensitive) == 0) {
            candidate = &account;
            break;
        }
    }

    if (!candidate) return {};
    if (candidate->getPasswordHash() != hashed) return {};
    if (!candidate->isActive()) return {};

    candidate->setLastLogin(core::DateTime::nowUtc());
    persist(accounts);
    return core::Optional(*candidate);
}

bool AccountService::createAccount(const core::CustomString &username, const core::CustomString &password, const core::CustomString &role, bool active) const {
    const core::Optional<core::CustomString> none;
    return createAccountInternal(username, password, role, active, none);
}

bool AccountService::createAccount(const core::CustomString &username, const core::CustomString &password, const core::CustomString &role, bool active,
                                   const core::CustomString &staffId) const {
    const core::CustomString cleanStaffId = staffId.trimmed();
    if (!cleanStaffId.isEmpty()) {
        if (!staffService.findById(cleanStaffId).has_value()) return false;
        if (isEmployeeIdInUse(cleanStaffId)) return false;
    }
    const core::Optional<core::CustomString> staffOpt =
        cleanStaffId.isEmpty() ? core::Optional<core::CustomString>() : core::Optional(cleanStaffId);
    return createAccountInternal(username, password, role, active, staffOpt);
}

bool AccountService::updateAccount(const model::Account &account) const {
    if (const core::CustomString newStaffId = account.getEmployeeId().trimmed(); !newStaffId.isEmpty()) {
        if (!staffService.findById(newStaffId).has_value()) return false;
        if (isEmployeeIdInUse(newStaffId, account.getUsername())) return false;
    }
    return updateItem(account);
}

bool AccountService::updatePassword(const core::CustomString &username, const core::CustomString &newPassword) const {
    const core::CustomString trimmed = username.trimmed();
    if (trimmed.isEmpty() || newPassword.isEmpty()) return false;

    auto accounts = ensureLoaded();
    bool updated = false;
    for (auto &account : accounts) {
        if (account.getUsername().compare(trimmed, core::CaseSensitivity::Insensitive) == 0) {
            account.setPasswordHash(repository::AccountsRepository::hashPassword(newPassword));
            updated = true;
            break;
        }
    }
    if (!updated) return false;
    persist(accounts);
    return true;
}

bool AccountService::removeAccount(const core::CustomString &username) const {
    return removeItem(username);
}

core::DynamicArray<model::Account> AccountService::ensureLoaded() const {
    auto accounts = repository.loadAll();
    core::DynamicArray<core::CustomString> ids;
    ids.reserve(accounts.size());
    bool mutated = false;
    for (auto &acc : accounts) {
        core::CustomString id = acc.getAccountId().trimmed();
        if (id.isEmpty()) {
            id = core::IdGenerator::nextId(ids, core::CustomStringLiteral("AC"), 3);
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

void AccountService::persist(const core::DynamicArray<model::Account> &accounts) const {
    repository.saveAll(accounts);
}

bool AccountService::isEmployeeIdInUse(const core::CustomString &staffId, const core::CustomString &excludeUsername) const {
    const core::CustomString trimmedStaff = staffId.trimmed();
    if (trimmedStaff.isEmpty()) return false;
    const core::CustomString trimmedExclude = excludeUsername.trimmed();

    const auto accounts = ensureLoaded();
    for (const auto &acc : accounts) {
        if (acc.getEmployeeId().trimmed().isEmpty()) continue;
        if (acc.getEmployeeId().compare(trimmedStaff, core::CaseSensitivity::Insensitive) != 0) continue;
        if (!trimmedExclude.isEmpty() && acc.getUsername().compare(trimmedExclude, core::CaseSensitivity::Insensitive) == 0) {
            continue;
        }
        return true;
    }
    return false;
}

bool AccountService::createAccountInternal(const core::CustomString &username, const core::CustomString &password, const core::CustomString &role, const bool active,
                                           const core::Optional<core::CustomString> &staffId) const {
    const core::CustomString cleanUsername = username.trimmed();
    const core::CustomString cleanRole = role.trimmed();
    const core::CustomString employee = staffId.has_value() ? staffId.value().trimmed() : core::CustomString();
    if (cleanUsername.isEmpty() || password.isEmpty()) return false;

    // Check if username exists using BaseService
    if (findById(cleanUsername).has_value()) {
        return false;
    }

    if (!employee.isEmpty() && isEmployeeIdInUse(employee)) {
        return false;
    }

    const auto accounts = ensureLoaded();
    core::DynamicArray<core::CustomString> ids;
    ids.reserve(accounts.size());
    for (const auto &account : accounts) {
        if (const core::CustomString existingId = account.getAccountId().trimmed(); !existingId.isEmpty()) {
            ids.pushBack(existingId);
        }
    }

    model::Account account;
    account.setAccountId(core::IdGenerator::nextId(ids, core::CustomStringLiteral("AC"), 3));
    account.setUsername(cleanUsername);
    account.setPasswordHash(repository::AccountsRepository::hashPassword(password));
    account.setRole(cleanRole.isEmpty() ? core::CustomStringLiteral("STAFF") : cleanRole);
    account.setActive(active);
    account.setCreatedAt(core::DateTime::nowUtc());
    account.setLastLogin(core::DateTime());
    if (!employee.isEmpty()) account.setEmployeeId(employee);

    return BaseService::addItem(account);
}

} // namespace pbl2::service
