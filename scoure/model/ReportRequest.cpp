#include "model/ReportRequest.h"
namespace pbl2::model {

const core::CustomString &ReportRequest::getRequestId() const {
    return requestId;
}

void ReportRequest::setRequestId(const core::CustomString &value) {
    requestId = value;
}

const core::CustomString &ReportRequest::getStaffUsername() const {
    return staffUsername;
}

void ReportRequest::setStaffUsername(const core::CustomString &value) {
    staffUsername = value;
}

const core::Date &ReportRequest::getFromDate() const {
    return fromDate;
}

void ReportRequest::setFromDate(const core::Date &value) {
    fromDate = value;
}

const core::Date &ReportRequest::getToDate() const {
    return toDate;
}

void ReportRequest::setToDate(const core::Date &value) {
    toDate = value;
}

int ReportRequest::getHandledLoans() const {
    return handledLoans;
}

void ReportRequest::setHandledLoans(const int value) {
    handledLoans = value;
}

int ReportRequest::getLostOrDamaged() const {
    return lostOrDamaged;
}

void ReportRequest::setLostOrDamaged(const int value) {
    lostOrDamaged = value;
}

int ReportRequest::getOverdueReaders() const {
    return overdueReaders;
}

void ReportRequest::setOverdueReaders(const int value) {
    overdueReaders = value;
}

const core::CustomString &ReportRequest::getAffectedBooks() const {
    return affectedBooks;
}

void ReportRequest::setAffectedBooks(const core::CustomString &value) {
    affectedBooks = value;
}

const core::CustomString &ReportRequest::getNotes() const {
    return notes;
}

void ReportRequest::setNotes(const core::CustomString &value) {
    notes = value;
}

const core::CustomString &ReportRequest::getStatus() const {
    return status;
}

void ReportRequest::setStatus(const core::CustomString &value) {
    status = value;
}

const core::DateTime &ReportRequest::getCreatedAt() const {
    return createdAt;
}

void ReportRequest::setCreatedAt(const core::DateTime &value) {
    createdAt = value;
}

}  // namespace pbl2::model
