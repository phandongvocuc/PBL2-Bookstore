#pragma once

#include "../../core/CustomString.h"
#include "../../core/DynamicArray.h"

#include "model/ReportRequest.h"
#include "repository/ReportRequestRepository.h"
#include "service/BaseService.h"

namespace pbl2::service {

    class ReportService final : public BaseService<model::ReportRequest, repository::ReportRequestRepository> {
    public:
        explicit ReportService(const core::CustomString &dataDir);

        [[nodiscard]] core::DynamicArray<model::ReportRequest> fetchAll() const override;
        [[nodiscard]] bool submitRequest(model::ReportRequest request) const;
        [[nodiscard]] bool updateStatus(const core::CustomString &requestId, const core::CustomString &status) const;
    private:
        [[nodiscard]] core::DynamicArray<model::ReportRequest> ensureLoaded() const override;
        void persist(const core::DynamicArray<model::ReportRequest> &requests) const override;
    };
    };// namespace pbl2::service
