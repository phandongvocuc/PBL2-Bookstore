#pragma once

#include "core/Date.h"
#include "core/DateTime.h"
#include "core/custom/CustomString.h"

namespace pbl2::model {

class ReportRequest {
public:
    [[nodiscard]] const custom::CustomString &getRequestId() const;
    void setRequestId(const custom::CustomString &value);

    [[nodiscard]] const custom::CustomString &getStaffUsername() const;
    void setStaffUsername(const custom::CustomString &value);

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

    [[nodiscard]] const custom::CustomString &getAffectedBooks() const;
    void setAffectedBooks(const custom::CustomString &value);

    [[nodiscard]] const custom::CustomString &getNotes() const;
    void setNotes(const custom::CustomString &value);

    [[nodiscard]] const custom::CustomString &getStatus() const;
    void setStatus(const custom::CustomString &value);

    [[nodiscard]] const core::DateTime &getCreatedAt() const;
    void setCreatedAt(const core::DateTime &value);

private:
    custom::CustomString requestId;
    custom::CustomString staffUsername;
    core::Date fromDate;
    core::Date toDate;
    int handledLoans{0};
    int lostOrDamaged{0};
    int overdueReaders{0};
    custom::CustomString affectedBooks;
    custom::CustomString notes;
    custom::CustomString status;
    core::DateTime createdAt;
};

}  // namespace model

