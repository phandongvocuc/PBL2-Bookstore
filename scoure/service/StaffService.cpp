#include "service/StaffService.h"

using namespace std;

namespace pbl2::service {

StaffService::StaffService(const custom::CustomString &dataDir) : repository(dataDir) {}

custom::DynamicArray<model::Staff> StaffService::fetchAll() const { return ensureLoaded(); }

custom::Optional<model::Staff> StaffService::findById(const custom::CustomString &staffId) const {
    const auto trimmed = staffId.trimmed();
    if (trimmed.isEmpty()) return {};
    const auto staffs = ensureLoaded();
    if (staffs.isEmpty()) return {};
    for (const auto & staff : staffs) {
        if (staff.getId().compare(trimmed, custom::CaseSensitivity::Insensitive) == 0) {
            return custom::Optional(staff);
        }
    }
    return {};
}

bool StaffService::addStaff(const model::Staff &staff) const {
    auto staffs = ensureLoaded();

    const model::Staff& copy = staff;
    bool exists = false;
    for (const auto & staff1 : staffs) {
        if (staff1.getId().compare(copy.getId(), custom::CaseSensitivity::Insensitive) == 0) {
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
    const auto staffs = ensureLoaded();
    bool updated = false;
    for (auto & it : staffs) {
        if (it.getId().compare(staff.getId(), custom::CaseSensitivity::Insensitive) == 0) {
            it = staff;
            updated = true;
            break;
        }
    }
    if (!updated) return false;
    persist(staffs);
    return true;
}

bool StaffService::setStaffActive(const custom::CustomString &staffId, const bool active) const {
    const auto staffs = ensureLoaded();
    bool changed = false;
    for (auto & staff : staffs) {
        if (staff.getId().compare(staffId, custom::CaseSensitivity::Insensitive) == 0) {
            if (staff.isActive() == active) return true;
            staff.setActive(active);
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
