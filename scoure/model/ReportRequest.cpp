#include "model/ReportRequest.h"
namespace pbl2::model {

const custom::CustomString &ReportRequest::getRequestId() const {
    return requestId;
}

void ReportRequest::setRequestId(const custom::CustomString &value) {
    requestId = value;
}

const custom::CustomString &ReportRequest::getStaffUsername() const {
    return staffUsername;
}

void ReportRequest::setStaffUsername(const custom::CustomString &value) {
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

const custom::CustomString &ReportRequest::getAffectedBooks() const {
    return affectedBooks;
}

void ReportRequest::setAffectedBooks(const custom::CustomString &value) {
    affectedBooks = value;
}

const custom::CustomString &ReportRequest::getNotes() const {
    return notes;
}

void ReportRequest::setNotes(const custom::CustomString &value) {
    notes = value;
}

const custom::CustomString &ReportRequest::getStatus() const {
    return status;
}

void ReportRequest::setStatus(const custom::CustomString &value) {
    status = value;
}

const core::DateTime &ReportRequest::getCreatedAt() const {
    return createdAt;
}

void ReportRequest::setCreatedAt(const core::DateTime &value) {
    createdAt = value;
}

}  // namespace pbl2::model
