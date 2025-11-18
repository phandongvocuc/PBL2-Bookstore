#pragma once

#include "core/custom/CustomString.h"

#include "core/custom/DynamicArray.h"

#include "model/Staff.h"

namespace pbl2::repository {

class StaffRepository {
public:
    explicit StaffRepository(const custom::CustomString &dataDir);

    custom::DynamicArray<model::Staff> loadAll() const;
    void saveAll(const custom::DynamicArray<model::Staff> &staffs) const;

private:
    custom::CustomString dataPath;
};

}  // namespace pbl2::repository
