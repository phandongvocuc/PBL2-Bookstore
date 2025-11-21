#pragma once

#include "core/custom/CustomString.h"
#include "core/Date.h"

namespace pbl2::model {

class Person {
public:
    virtual ~Person() = default;

    [[nodiscard]] const custom::CustomString &getId() const;
    void setId(const custom::CustomString &value);

    [[nodiscard]] const custom::CustomString &getFullName() const;
    void setFullName(const custom::CustomString &value);

    [[nodiscard]] const core::Date &getDob() const;
    void setDob(const core::Date &value);

    [[nodiscard]] bool isActive() const;
    void setActive(bool value);

protected:
    Person() = default;

private:
    custom::CustomString id;
    custom::CustomString fullName;
    core::Date dob;
    bool active{false};
};

}  // namespace pbl2::model

