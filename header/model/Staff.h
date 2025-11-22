#pragma once

#include "core/Date.h"
#include "core/custom/CustomString.h"

#include "Person.h"

namespace pbl2::model {

    struct Staff final : Person {
        ~Staff() override = default;

        [[nodiscard]] const core::Date &getHireDate() const;
        void setHireDate(const core::Date &value);

        [[nodiscard]] const custom::CustomString &getPosition() const;
        void setPosition(const custom::CustomString &value);

        [[nodiscard]] const custom::CustomString &getNotes() const;
        void setNotes(const custom::CustomString &value);

    private:
        core::Date hireDate;
        custom::CustomString position;
        custom::CustomString notes;
    };

}  // namespace pbl2::model