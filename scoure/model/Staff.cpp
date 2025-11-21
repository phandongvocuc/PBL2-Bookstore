#include "model/Staff.h"
using namespace std;
namespace pbl2::model {

const custom::CustomString &Staff::getGender() const {
    return gender;
}

void Staff::setGender(const custom::CustomString &value) {
    gender = value;
}

const custom::CustomString &Staff::getEmail() const {
    return email;
}

void Staff::setEmail(const custom::CustomString &value) {
    email = value;
}

const custom::CustomString &Staff::getAddress() const {
    return address;
}

void Staff::setAddress(const custom::CustomString &value) {
    address = value;
}

const custom::CustomString &Staff::getPhone() const {
    return phone;
}

void Staff::setPhone(const custom::CustomString &value) {
    phone = value;
}

const core::Date &Staff::getHireDate() const {
    return hireDate;
}

void Staff::setHireDate(const core::Date &value) {
    hireDate = value;
}

const custom::CustomString &Staff::getPosition() const {
    return position;
}

void Staff::setPosition(const custom::CustomString &value) {
    position = value;
}

const custom::CustomString &Staff::getNotes() const {
    return notes;
}

void Staff::setNotes(const custom::CustomString &value) {
    notes = value;
}

}  // namespace pbl2::model
