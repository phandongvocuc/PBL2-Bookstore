#include "model/Person.h"

using namespace std;

namespace pbl2::model {

    const core::CustomString &Person::getId() const {
        return id;
    }

    void Person::setId(const core::CustomString &value) {
        id = value;
    }

    const core::CustomString &Person::getFullName() const {
        return fullName;
    }

    void Person::setFullName(const core::CustomString &value) {
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

    const core::CustomString &Person::getGender() const {
        return gender;
    }

    void Person::setGender(const core::CustomString &value) {
        gender = value;
    }

    const core::CustomString &Person::getEmail() const {
        return email;
    }

    void Person::setEmail(const core::CustomString &value) {
        email = value;
    }

    const core::CustomString &Person::getAddress() const {
        return address;
    }

    void Person::setAddress(const core::CustomString &value) {
        address = value;
    }

    const core::CustomString &Person::getPhone() const {
        return phone;
    }

    void Person::setPhone(const core::CustomString &value) {
        phone = value;
    }

}  // namespace pbl2::model