#pragma once

#include "core/DateTime.h"
#include "core/custom/CustomString.h"

#include "Person.h"
using namespace std;

namespace pbl2::model {

class Account final : public Person {
public:
    ~Account() override = default;

    [[nodiscard]] const custom::CustomString &getAccountId() const;
    void setAccountId(const custom::CustomString &value);

    [[nodiscard]] const custom::CustomString &getUsername() const;
    void setUsername(const custom::CustomString &value);

    [[nodiscard]] const custom::CustomString &getPasswordHash() const;
    void setPasswordHash(const custom::CustomString &value);

    [[nodiscard]] const custom::CustomString &getRole() const;
    void setRole(const custom::CustomString &value);

    [[nodiscard]] const custom::CustomString &getEmployeeId() const;
    void setEmployeeId(const custom::CustomString &value);

    [[nodiscard]] const core::DateTime &getCreatedAt() const;
    void setCreatedAt(const core::DateTime &value);

    [[nodiscard]] const core::DateTime &getLastLogin() const;
    void setLastLogin(const core::DateTime &value);

private:
    custom::CustomString username;
    custom::CustomString passwordHash;
    custom::CustomString role;
    custom::CustomString employeeId;
    core::DateTime createdAt;
    core::DateTime lastLogin;
};

}  // namespace pbl2::model
