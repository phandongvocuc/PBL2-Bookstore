#include "service/StaffService.h"

namespace pbl2::service {

    StaffService::StaffService(const custom::CustomString &dataDir)
        : BaseService(dataDir) {}

    custom::DynamicArray<model::Staff> StaffService::fetchAll() const {
        return BaseService::fetchAll();
    }

    custom::Optional<model::Staff> StaffService::findById(const custom::CustomString &staffId) const {
        return BaseService::findById(staffId);
    }

    bool StaffService::addStaff(const model::Staff &staff) const {
        return addItem(staff);
    }

    bool StaffService::updateStaff(const model::Staff &staff) const {
        return updateItem(staff);
    }

    bool StaffService::setStaffActive(const custom::CustomString &staffId, bool active) const {
        const auto staffs = ensureLoaded();
        bool changed = false;
        for (auto &staff : staffs) {
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
        return removeItem(staffId);
    }

    custom::DynamicArray<model::Staff> StaffService::ensureLoaded() const {
        return repository.loadAll();
    }

    void StaffService::persist(const custom::DynamicArray<model::Staff> &staffs) const {
        repository.saveAll(staffs);
    }

} // namespace pbl2::service