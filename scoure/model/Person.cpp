#include "model/Person.h"

using namespace std;

namespace pbl2::model {

    const custom::CustomString &Person::getId() const {
        return id;
    }

    void Person::setId(const custom::CustomString &value) {
        id = value;
    }

    const custom::CustomString &Person::getFullName() const {
        return fullName;
    }

    void Person::setFullName(const custom::CustomString &value) {
        fullName = value;
    }

    const core::Date &Person::getDob() const {
        return dob;
    }

    void Person::setDob(const core::Date &value) {
        dob = value;
    }

    bool Person::isActive() const {
        return active;
    }

    void Person::setActive(const bool value) {
        active = value;
    }

    const custom::CustomString &Person::getGender() const {
        return gender;
    }

    void Person::setGender(const custom::CustomString &value) {
        gender = value;
    }

    const custom::CustomString &Person::getEmail() const {
        return email;
    }

    void Person::setEmail(const custom::CustomString &value) {
        email = value;
    }

    const custom::CustomString &Person::getAddress() const {
        return address;
    }

    void Person::setAddress(const custom::CustomString &value) {
        address = value;
    }

    const custom::CustomString &Person::getPhone() const {
        return phone;
    }

    void Person::setPhone(const custom::CustomString &value) {
        phone = value;
    }

}  // namespace pbl2::model