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

}  // namespace pbl2::model
