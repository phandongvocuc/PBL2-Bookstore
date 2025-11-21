#include "model/Reader.h"
using namespace std;
namespace pbl2::model {

const custom::CustomString &Reader::getGender() const {
    return gender;
}

void Reader::setGender(const custom::CustomString &value) {
    gender = value;
}

const custom::CustomString &Reader::getEmail() const {
    return email;
}

void Reader::setEmail(const custom::CustomString &value) {
    email = value;
}

const custom::CustomString &Reader::getAddress() const {
    return address;
}

void Reader::setAddress(const custom::CustomString &value) {
    address = value;
}

const custom::CustomString &Reader::getPhone() const {
    return phone;
}

void Reader::setPhone(const custom::CustomString &value) {
    phone = value;
}

const custom::CustomString &Reader::getIdentityCard() const {
    return identityCard;
}

void Reader::setIdentityCard(const custom::CustomString &value) {
    identityCard = value;
}

const core::Date &Reader::getCreatedDate() const {
    return createdDate;
}

void Reader::setCreatedDate(const core::Date &value) {
    createdDate = value;
}

const core::Date &Reader::getExpiryDate() const {
    return expiryDate;
}

void Reader::setExpiryDate(const core::Date &value) {
    expiryDate = value;
}

int Reader::getTotalBorrowed() const {
    return totalBorrowed;
}

void Reader::setTotalBorrowed(const int value) {
    totalBorrowed = value;
}

const custom::CustomString &Reader::getNotes() const {
    return notes;
}

void Reader::setNotes(const custom::CustomString &value) {
    notes = value;
}

}  // namespace pbl2::model
