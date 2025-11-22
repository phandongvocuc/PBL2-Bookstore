#pragma once

#include "../../core/CustomString.h"
#include "core/Date.h"

namespace pbl2::model {

    class Person {
    public:
        virtual ~Person() = default;

        [[nodiscard]] const core::CustomString &getId() const;
        void setId(const core::CustomString &value);

        [[nodiscard]] const core::CustomString &getFullName() const;
        void setFullName(const core::CustomString &value);

        [[nodiscard]] const core::Date &getDob() const;
        void setDob(const core::Date &value);

        [[nodiscard]] bool isActive() const;
        void setActive(bool value);

        [[nodiscard]] const core::CustomString &getGender() const;
        void setGender(const core::CustomString &value);

        [[nodiscard]] const core::CustomString &getEmail() const;
        void setEmail(const core::CustomString &value);

        [[nodiscard]] const core::CustomString &getAddress() const;
        void setAddress(const core::CustomString &value);

        [[nodiscard]] const core::CustomString &getPhone() const;
        void setPhone(const core::CustomString &value);

    protected:
        Person() = default;

        core::CustomString gender;
        core::CustomString email;
        core::CustomString address;
        core::CustomString phone;

    private:
        core::CustomString id;
        core::CustomString fullName;
        core::Date dob;
        bool active{false};
    };

}  // namespace pbl2::model