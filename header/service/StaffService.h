#pragma once

#include "core/custom/Optional.h"
#include "core/custom/CustomString.h"
#include "core/custom/DynamicArray.h"

#include "model/Staff.h"
#include "repository/StaffRepository.h"
#include "service/BaseService.h"

namespace pbl2::service {

    class StaffService : public BaseService<model::Staff, repository::StaffRepository> {
    public:
        explicit StaffService(const custom::CustomString &dataDir);

        [[nodiscard]] custom::DynamicArray<model::Staff> fetchAll() const override;
        [[nodiscard]] custom::Optional<model::Staff> findById(const custom::CustomString &staffId) const override;
        [[nodiscard]] bool addStaff(const model::Staff &staff) const;
        [[nodiscard]] bool updateStaff(const model::Staff &staff) const;
        [[nodiscard]] bool setStaffActive(const custom::CustomString &staffId, bool active) const;
        [[nodiscard]] bool removeStaff(const custom::CustomString &staffId) const;

    private:
        [[nodiscard]] custom::DynamicArray<model::Staff> ensureLoaded() const override;
        void persist(const custom::DynamicArray<model::Staff> &staffs) const override;
    };

} // namespace pbl2::service