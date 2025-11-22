#pragma once

#include "core/custom/CustomString.h"
#include "core/custom/DynamicArray.h"

#include "model/ReportRequest.h"
#include "repository/ReportRequestRepository.h"
#include "service/BaseService.h"

namespace pbl2::service {

    class ReportService final : public BaseService<model::ReportRequest, repository::ReportRequestRepository> {
    public:
        explicit ReportService(const custom::CustomString &dataDir);

        [[nodiscard]] custom::DynamicArray<model::ReportRequest> fetchAll() const override;
        [[nodiscard]] bool submitRequest(model::ReportRequest request) const;
        [[nodiscard]] bool updateStatus(const custom::CustomString &requestId, const custom::CustomString &status) const;
    private:
        [[nodiscard]] custom::DynamicArray<model::ReportRequest> ensureLoaded() const override;
        void persist(const custom::DynamicArray<model::ReportRequest> &requests) const override;
    };
    };// namespace pbl2::service

