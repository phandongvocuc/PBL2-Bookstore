#pragma once

#include "core/custom/CustomString.h"

#include "core/custom/DynamicArray.h"

#include "model/ReportRequest.h"

namespace pbl2::repository {

class ReportRequestRepository {
public:
    explicit ReportRequestRepository(const custom::CustomString &dataDir);

    [[nodiscard]] custom::DynamicArray<model::ReportRequest> loadAll() const;
    void saveAll(const custom::DynamicArray<model::ReportRequest> &requests) const;

private:
    custom::CustomString dataPath;
};

}  // namespace pbl2::repository
