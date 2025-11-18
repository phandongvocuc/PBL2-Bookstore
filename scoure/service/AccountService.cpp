#include "service/AccountService.h"

#include "core/DateTime.h"
#include "core/IdGenerator.h"

using namespace std;

namespace pbl2::service {

AccountService::AccountService(const custom::CustomString &dataDir)
    : repository(dataDir), staffService(dataDir) {}

custom::DynamicArray<model::Account> AccountService::fetchAll() const { return ensureLoaded(); }

custom::Optional<model::Account> AccountService::findByUsername(const custom::CustomString &username) const {
    const custom::CustomString trimmed = username.trimmed();
    if (trimmed.isEmpty()) return custom::Optional<model::Account>();
    const auto accounts = ensureLoaded();
    if (accounts.isEmpty()) return custom::Optional<model::Account>();
    for (custom::DynamicArray<model::Account>::ConstIterator it = accounts.cbegin(); it != accounts.cend(); ++it) {
        if (it->getUsername().compare(trimmed, custom::CaseSensitivity::Insensitive) == 0) {
            return custom::Optional(*it);
        }
    }
    return custom::Optional<model::Account>();
}

custom::Optional<model::Account> AccountService::authenticate(const custom::CustomString &username, const custom::CustomString &password) const {
    const custom::CustomString trimmed = username.trimmed();
    if (trimmed.isEmpty() || password.isEmpty()) return custom::Optional<model::Account>();
    auto accounts = ensureLoaded();
    if (accounts.isEmpty()) return custom::Optional<model::Account>();
    const custom::CustomString hashed = repository::AccountsRepository::hashPassword(password);
    model::Account *candidate = nullptr;
    for (custom::DynamicArray<model::Account>::Iterator it = accounts.begin(); it != accounts.end(); ++it) {
        if (it->getUsername().compare(trimmed, custom::CaseSensitivity::Insensitive) == 0) {
            candidate = it;
            break;
        }
    }
    if (!candidate) return custom::Optional<model::Account>();
    if (candidate->getPasswordHash() != hashed) return custom::Optional<model::Account>();
    if (!candidate->isActive()) return custom::Optional<model::Account>();

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

    auto accounts = ensureLoaded();
    bool updated = false;
    for (custom::DynamicArray<model::Account>::Iterator it = accounts.begin(); it != accounts.end(); ++it) {
        if (it->getUsername().compare(account.getUsername(), custom::CaseSensitivity::Insensitive) == 0) {
            *it = account;
            updated = true;
            break;
        }
    }
    if (!updated) return false;
    persist(accounts);
    return true;
}

bool AccountService::updatePassword(const custom::CustomString &username, const custom::CustomString &newPassword) const {
    const custom::CustomString trimmed = username.trimmed();
    if (trimmed.isEmpty() || newPassword.isEmpty()) return false;
    auto accounts = ensureLoaded();
    bool updated = false;
    for (custom::DynamicArray<model::Account>::Iterator it = accounts.begin(); it != accounts.end(); ++it) {
        if (it->getUsername().compare(trimmed, custom::CaseSensitivity::Insensitive) == 0) {
            it->setPasswordHash(repository::AccountsRepository::hashPassword(newPassword));
            updated = true;
            break;
        }
    }
    if (!updated) return false;
    persist(accounts);
    return true;
}

bool AccountService::removeAccount(const custom::CustomString &username) const {
    const custom::CustomString trimmed = username.trimmed();
    if (trimmed.isEmpty()) return false;
    auto accounts = ensureLoaded();
    bool removed = false;
    custom::DynamicArray<model::Account>::SizeType index = 0U;
    while (index < accounts.size()) {
        if (accounts[index].getUsername().compare(trimmed, custom::CaseSensitivity::Insensitive) == 0) {
            accounts.removeAt(index);
            removed = true;
        } else {
            ++index;
        }
    }
    if (!removed) return false;
    persist(accounts);
    return true;
}

custom::DynamicArray<model::Account> AccountService::ensureLoaded() const {
    auto accounts = repository.loadAll();
    custom::DynamicArray<custom::CustomString> ids;
    ids.reserve(accounts.size());
    bool mutated = false;
    for (custom::DynamicArray<model::Account>::SizeType i = 0U; i < accounts.size(); ++i) {
        model::Account &acc = accounts[i];
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
    const auto accounts = ensureLoaded();
    for (custom::DynamicArray<model::Account>::ConstIterator it = accounts.cbegin(); it != accounts.cend(); ++it) {
        const model::Account &acc = *it;
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

    auto accounts = ensureLoaded();
    bool usernameExists = false;
    for (custom::DynamicArray<model::Account>::ConstIterator it = accounts.cbegin(); it != accounts.cend(); ++it) {
        if (it->getUsername().compare(cleanUsername, custom::CaseSensitivity::Insensitive) == 0) {
            usernameExists = true;
            break;
        }
    }
    if (usernameExists) return false;

    if (!employee.isEmpty()) {
        bool employeeUsed = false;
        for (custom::DynamicArray<model::Account>::ConstIterator it = accounts.cbegin(); it != accounts.cend(); ++it) {
            if (it->getEmployeeId().trimmed().isEmpty()) {
                continue;
            }
            if (it->getEmployeeId().compare(employee, custom::CaseSensitivity::Insensitive) == 0) {
                employeeUsed = true;
                break;
            }
        }
        if (employeeUsed) return false;
    }

    custom::DynamicArray<custom::CustomString> ids;
    ids.reserve(accounts.size());
    for (custom::DynamicArray<model::Account>::ConstIterator it = accounts.cbegin(); it != accounts.cend(); ++it) {
        if (const custom::CustomString existingId = it->getAccountId().trimmed(); !existingId.isEmpty()) ids.pushBack(existingId);
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

    accounts.pushBack(account);
    persist(accounts);
    return true;
}

}  // namespace pbl2::service
