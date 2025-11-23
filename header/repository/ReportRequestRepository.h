#pragma once

#include "../../core/CustomString.h"
#include "../../core/DynamicArray.h"

#include "model/ReportRequest.h"

namespace pbl2::repository {

class ReportRequestRepository {
public:
    explicit ReportRequestRepository(const core::CustomString &dataDir);

    [[nodiscard]] core::DynamicArray<model::ReportRequest> loadAll() const;
    void saveAll(const core::DynamicArray<model::ReportRequest> &requests) const;

private:
    core::CustomString dataPath;
};

}  // namespace pbl2::repository
