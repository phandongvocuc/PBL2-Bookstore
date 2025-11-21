#include "model/Account.h"
using namespace std;

namespace pbl2::model {

const custom::CustomString &Account::getAccountId() const {
    return getId();
}

void Account::setAccountId(const custom::CustomString &value) {
    setId(value);
}

const custom::CustomString &Account::getUsername() const {
    return username;
}

void Account::setUsername(const custom::CustomString &value) {
    username = value;
}

const custom::CustomString &Account::getPasswordHash() const {
    return passwordHash;
}

void Account::setPasswordHash(const custom::CustomString &value) {
    passwordHash = value;
}

const custom::CustomString &Account::getRole() const {
    return role;
}

void Account::setRole(const custom::CustomString &value) {
    role = value;
}

const custom::CustomString &Account::getEmployeeId() const {
    return employeeId;
}

void Account::setEmployeeId(const custom::CustomString &value) {
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
