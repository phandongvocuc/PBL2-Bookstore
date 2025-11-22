#pragma once

#include "core/Date.h"
#include "core/custom/CustomString.h"
#include "Person.h"

namespace pbl2::model {

    struct Reader final : Person {
    public:
        ~Reader() override = default;

        [[nodiscard]] const custom::CustomString &getIdentityCard() const;
        void setIdentityCard(const custom::CustomString &value);

        [[nodiscard]] const core::Date &getCreatedDate() const;
        void setCreatedDate(const core::Date &value);

        [[nodiscard]] const core::Date &getExpiryDate() const;
        void setExpiryDate(const core::Date &value);

        [[nodiscard]] int getTotalBorrowed() const;
        void setTotalBorrowed(int value);

        [[nodiscard]] const custom::CustomString &getNotes() const;
        void setNotes(const custom::CustomString &value);

    private:
        custom::CustomString identityCard;
        core::Date createdDate;
        core::Date expiryDate;
        int totalBorrowed{0};
        custom::CustomString notes;
    };

}  // namespace model