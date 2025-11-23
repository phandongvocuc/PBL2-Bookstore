#pragma once

#include "core/Date.h"
#include "core/DateTime.h"
#include "../../core/CustomString.h"

namespace pbl2::model {

class ReportRequest {
public:
    [[nodiscard]] const core::CustomString &getRequestId() const;
    void setRequestId(const core::CustomString &value);

    [[nodiscard]] const core::CustomString &getStaffUsername() const;
    void setStaffUsername(const core::CustomString &value);

    [[nodiscard]] const core::Date &getFromDate() const;
    void setFromDate(const core::Date &value);

    [[nodiscard]] const core::Date &getToDate() const;
    void setToDate(const core::Date &value);

    [[nodiscard]] int getHandledLoans() const;
    void setHandledLoans(int value);

    [[nodiscard]] int getLostOrDamaged() const;
    void setLostOrDamaged(int value);

    [[nodiscard]] int getOverdueReaders() const;
    void setOverdueReaders(int value);

    [[nodiscard]] const core::CustomString &getAffectedBooks() const;
    void setAffectedBooks(const core::CustomString &value);

    [[nodiscard]] const core::CustomString &getNotes() const;
    void setNotes(const core::CustomString &value);

    [[nodiscard]] const core::CustomString &getStatus() const;
    void setStatus(const core::CustomString &value);

    [[nodiscard]] const core::DateTime &getCreatedAt() const;
    void setCreatedAt(const core::DateTime &value);

private:
    core::CustomString requestId;
    core::CustomString staffUsername;
    core::Date fromDate;
    core::Date toDate;
    int handledLoans{0};
    int lostOrDamaged{0};
    int overdueReaders{0};
    core::CustomString affectedBooks;
    core::CustomString notes;
    core::CustomString status;
    core::DateTime createdAt;
};

}  // namespace model

