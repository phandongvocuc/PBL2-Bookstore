#pragma once

#include "core/DateTime.h"
#include "../../core/CustomString.h"

#include "Person.h"
using namespace std;

namespace pbl2::model {

class Account final : public Person {
public:
    ~Account() override = default;

    [[nodiscard]] const core::CustomString &getAccountId() const;
    void setAccountId(const core::CustomString &value);

    [[nodiscard]] const core::CustomString &getUsername() const;
    void setUsername(const core::CustomString &value);

    [[nodiscard]] const core::CustomString &getPasswordHash() const;
    void setPasswordHash(const core::CustomString &value);

    [[nodiscard]] const core::CustomString &getRole() const;
    void setRole(const core::CustomString &value);

    [[nodiscard]] const core::CustomString &getEmployeeId() const;
    void setEmployeeId(const core::CustomString &value);

    [[nodiscard]] const core::DateTime &getCreatedAt() const;
    void setCreatedAt(const core::DateTime &value);

    [[nodiscard]] const core::DateTime &getLastLogin() const;
    void setLastLogin(const core::DateTime &value);

private:
    core::CustomString username;
    core::CustomString passwordHash;
    core::CustomString role;
    core::CustomString employeeId;
    core::DateTime createdAt;
    core::DateTime lastLogin;
};

}  // namespace pbl2::model
