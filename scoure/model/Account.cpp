#include "model/Account.h"
using namespace std;

namespace pbl2::model {

const core::CustomString &Account::getAccountId() const {
    return getId();
}

void Account::setAccountId(const core::CustomString &value) {
    setId(value);
}

const core::CustomString &Account::getUsername() const {
    return username;
}

void Account::setUsername(const core::CustomString &value) {
    username = value;
}

const core::CustomString &Account::getPasswordHash() const {
    return passwordHash;
}

void Account::setPasswordHash(const core::CustomString &value) {
    passwordHash = value;
}

const core::CustomString &Account::getRole() const {
    return role;
}

void Account::setRole(const core::CustomString &value) {
    role = value;
}

const core::CustomString &Account::getEmployeeId() const {
    return employeeId;
}

void Account::setEmployeeId(const core::CustomString &value) {
    employeeId = value;
}

const core::DateTime &Account::getCreatedAt() const {
    return createdAt;
}

void Account::setCreatedAt(const core::DateTime &value) {
    createdAt = value;
}

const core::DateTime &Account::getLastLogin() const {
    return lastLogin;
}

void Account::setLastLogin(const core::DateTime &value) {
    lastLogin = value;
}

}  // namespace pbl2::model
