#pragma once

#include "../../core/CustomString.h"
#include "../../core/DynamicArray.h"

#include "model/Staff.h"

namespace pbl2::repository {

class StaffRepository {
public:
    explicit StaffRepository(const core::CustomString &dataDir);

    [[nodiscard]] core::DynamicArray<model::Staff> loadAll() const;
    void saveAll(const core::DynamicArray<model::Staff> &staffs) const;

private:
    core::CustomString dataPath;
};

}  // namespace pbl2::repository
