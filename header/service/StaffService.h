#pragma once

#include "../../core/Optional.h"
#include "../../core/CustomString.h"
#include "../../core/DynamicArray.h"

#include "model/Staff.h"
#include "repository/StaffRepository.h"
#include "service/BaseService.h"

namespace pbl2::service {

    class StaffService : public BaseService<model::Staff, repository::StaffRepository> {
    public:
        explicit StaffService(const core::CustomString &dataDir);

        [[nodiscard]] core::DynamicArray<model::Staff> fetchAll() const override;
        [[nodiscard]] core::Optional<model::Staff> findById(const core::CustomString &staffId) const override;
        [[nodiscard]] bool addStaff(const model::Staff &staff) const;
        [[nodiscard]] bool updateStaff(const model::Staff &staff) const;
        [[nodiscard]] bool setStaffActive(const core::CustomString &staffId, bool active) const;
        [[nodiscard]] bool removeStaff(const core::CustomString &staffId) const;

    private:
        [[nodiscard]] core::DynamicArray<model::Staff> ensureLoaded() const override;
        void persist(const core::DynamicArray<model::Staff> &staffs) const override;
    public:
        // Hàm kiểm tra trùng lặp số điện thoại
        [[nodiscard]] bool isDuplicatePhone(const core::CustomString &phone) const;
    };

} // namespace pbl2::service
