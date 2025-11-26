#include "service/StaffService.h"

namespace pbl2::service {

    bool StaffService::isDuplicatePhone(const core::CustomString &phone) const {
        auto staffs = ensureLoaded();
        for (const auto &staff : staffs) {
            if (staff.getPhone().compare(phone, core::CaseSensitivity::Insensitive) == 0) {
                return true;
            }
        }
        return false;
    }
    StaffService::StaffService(const core::CustomString &dataDir)
        : BaseService(dataDir) {}

    core::DynamicArray<model::Staff> StaffService::fetchAll() const {
        return BaseService::fetchAll();
    }

    core::Optional<model::Staff> StaffService::findById(const core::CustomString &staffId) const {
        return BaseService::findById(staffId);
    }

    bool StaffService::addStaff(const model::Staff &staff) const {
        return addItem(staff);
    }

    bool StaffService::updateStaff(const model::Staff &staff) const {
        return updateItem(staff);
    }

    bool StaffService::setStaffActive(const core::CustomString &staffId, bool active) const {
        auto staffs = ensureLoaded();
        bool changed = false;
        for (auto &staff : staffs) {
            if (staff.getId().compare(staffId, core::CaseSensitivity::Insensitive) == 0) {
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

    bool StaffService::removeStaff(const core::CustomString &staffId) const {
        return removeItem(staffId);
    }

    core::DynamicArray<model::Staff> StaffService::ensureLoaded() const {
        return repository.loadAll();
    }

    void StaffService::persist(const core::DynamicArray<model::Staff> &staffs) const {
        repository.saveAll(staffs);
    }

} // namespace pbl2::service
