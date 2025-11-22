#pragma once

#include "core/Date.h"
#include "../../core/CustomString.h"

#include "Person.h"

namespace pbl2::model {

    struct Staff final : Person {
        ~Staff() override = default;

        [[nodiscard]] const core::Date &getHireDate() const;
        void setHireDate(const core::Date &value);

        [[nodiscard]] const core::CustomString &getPosition() const;
        void setPosition(const core::CustomString &value);

        [[nodiscard]] const core::CustomString &getNotes() const;
        void setNotes(const core::CustomString &value);

    private:
        core::Date hireDate;
        core::CustomString position;
        core::CustomString notes;
    };

}  // namespace pbl2::model