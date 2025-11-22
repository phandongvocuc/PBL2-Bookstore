#pragma once

#include "core/Date.h"
#include "../../core/CustomString.h"
#include "Person.h"

namespace pbl2::model {

    struct Reader final : Person {
    public:
        ~Reader() override = default;

        [[nodiscard]] const core::CustomString &getIdentityCard() const;
        void setIdentityCard(const core::CustomString &value);

        [[nodiscard]] const core::Date &getCreatedDate() const;
        void setCreatedDate(const core::Date &value);

        [[nodiscard]] const core::Date &getExpiryDate() const;
        void setExpiryDate(const core::Date &value);

        [[nodiscard]] int getTotalBorrowed() const;
        void setTotalBorrowed(int value);

        [[nodiscard]] const core::CustomString &getNotes() const;
        void setNotes(const core::CustomString &value);

    private:
        core::CustomString identityCard;
        core::Date createdDate;
        core::Date expiryDate;
        int totalBorrowed{0};
        core::CustomString notes;
    };

}  // namespace model