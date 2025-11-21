#pragma once

#include "core/custom/CustomString.h"

#include "core/custom/DynamicArray.h"

#include "model/ReportRequest.h"
#include "repository/ReportRequestRepository.h"

namespace pbl2::service {

class ReportService {
public:
    explicit ReportService(const custom::CustomString &dataDir);

    [[nodiscard]] custom::DynamicArray<model::ReportRequest> fetchAll() const;

    [[nodiscard]] bool submitRequest(model::ReportRequest request) const;
    [[nodiscard]] bool updateStatus(const custom::CustomString &requestId, const custom::CustomString &status) const;

private:
    repository::ReportRequestRepository repository;

    [[nodiscard]] custom::DynamicArray<model::ReportRequest> ensureLoaded() const;
    void persist(const custom::DynamicArray<model::ReportRequest> &requests) const;
};

}  // namespace pbl2::service
