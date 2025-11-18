#include "service/StaffService.h"

using namespace std;

namespace pbl2::service {

StaffService::StaffService(const custom::CustomString &dataDir) : repository(dataDir) {}

custom::DynamicArray<model::Staff> StaffService::fetchAll() const { return ensureLoaded(); }

custom::Optional<model::Staff> StaffService::findById(const custom::CustomString &staffId) const {
    const auto trimmed = staffId.trimmed();
    if (trimmed.isEmpty()) return custom::Optional<model::Staff>();
    const auto staffs = ensureLoaded();
    if (staffs.isEmpty()) return custom::Optional<model::Staff>();
    for (custom::DynamicArray<model::Staff>::ConstIterator it = staffs.cbegin(); it != staffs.cend(); ++it) {
        if (it->getId().compare(trimmed, custom::CaseSensitivity::Insensitive) == 0) {
            return custom::Optional(*it);
        }
    }
    return custom::Optional<model::Staff>();
}

bool StaffService::addStaff(const model::Staff &staff) const {
    auto staffs = ensureLoaded();

    const model::Staff copy = staff;
    bool exists = false;
    for (custom::DynamicArray<model::Staff>::ConstIterator it = staffs.cbegin(); it != staffs.cend(); ++it) {
        if (it->getId().compare(copy.getId(), custom::CaseSensitivity::Insensitive) == 0) {
            exists = true;
            break;
        }
    }
    if (exists) return false;

    staffs.pushBack(copy);
    persist(staffs);
    return true;
}

bool StaffService::updateStaff(const model::Staff &staff) const {
    auto staffs = ensureLoaded();
    bool updated = false;
    for (custom::DynamicArray<model::Staff>::Iterator it = staffs.begin(); it != staffs.end(); ++it) {
        if (it->getId().compare(staff.getId(), custom::CaseSensitivity::Insensitive) == 0) {
            *it = staff;
            updated = true;
            break;
        }
    }
    if (!updated) return false;
    persist(staffs);
    return true;
}

bool StaffService::setStaffActive(const custom::CustomString &staffId, const bool active) const {
    auto staffs = ensureLoaded();
    bool changed = false;
    for (custom::DynamicArray<model::Staff>::Iterator it = staffs.begin(); it != staffs.end(); ++it) {
        if (it->getId().compare(staffId, custom::CaseSensitivity::Insensitive) == 0) {
            if (it->isActive() == active) return true;
            it->setActive(active);
            changed = true;
            break;
        }
    }
    if (!changed) return false;
    persist(staffs);
    return true;
}

bool StaffService::removeStaff(const custom::CustomString &staffId) const {
    auto staffs = ensureLoaded();
    bool removed = false;
    custom::DynamicArray<model::Staff>::SizeType index = 0U;
    while (index < staffs.size()) {
        if (staffs[index].getId().compare(staffId, custom::CaseSensitivity::Insensitive) == 0) {
            staffs.removeAt(index);
            removed = true;
        } else {
            ++index;
        }
    }
    if (!removed) return false;
    persist(staffs);
    return true;
}

custom::DynamicArray<model::Staff> StaffService::ensureLoaded() const {
    return repository.loadAll();
}

void StaffService::persist(const custom::DynamicArray<model::Staff> &staffs) const {
    repository.saveAll(staffs);
}

}  // namespace pbl2::service
