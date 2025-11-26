#include "model/Staff.h"
namespace pbl2::model {

    const core::Date &Staff::getHireDate() const {
        return hireDate;
    }

    void Staff::setHireDate(const core::Date &value) {
        hireDate = value;
    }

    const core::CustomString &Staff::getPosition() const {
        return position;
    }

    void Staff::setPosition(const core::CustomString &value) {
        position = value;
    }

    const core::CustomString &Staff::getNotes() const {
        return notes;
    }

    void Staff::setNotes(const core::CustomString &value) {
        notes = value;
    }

}  // namespace pbl2::model