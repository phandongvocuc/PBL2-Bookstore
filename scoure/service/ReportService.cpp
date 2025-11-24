#include "service/ReportService.h"
#include "core/DateTime.h"

namespace pbl2::service {

    ReportService::ReportService(const core::CustomString &dataDir)
        : BaseService(dataDir) {}

    core::DynamicArray<model::ReportRequest> ReportService::fetchAll() const {
        return BaseService::fetchAll();
    }

    bool ReportService::submitRequest(model::ReportRequest request) const {
        auto requests = ensureLoaded();

        if (request.getRequestId().trimmed().isEmpty()) {
            const auto now = core::DateTime::nowUtc();
            const core::CustomString timestamp = now.toCompactTimestamp();
            core::CustomString id(core::CustomStringLiteral("RR-"));
            id.append(timestamp);
            request.setRequestId(id);
            request.setCreatedAt(now);
        }

        // Check for duplicates using BaseService logic
        if (findById(request.getRequestId()).has_value()) {
            return false;
        }

        if (!request.getCreatedAt().isValid()) {
            request.setCreatedAt(core::DateTime::nowUtc());
        }
        if (request.getStatus().trimmed().isEmpty()) {
            request.setStatus(core::CustomStringLiteral("PENDING"));
        }

        return addItem(request);
    }

    bool ReportService::updateStatus(const core::CustomString &requestId, const core::CustomString &status) const {
        auto requests = ensureLoaded();
        bool changed = false;
        for (auto &request : requests) {
            if (request.getRequestId().compare(requestId, core::CaseSensitivity::Insensitive) == 0) {
                const core::CustomString trimmed = status.trimmed();
                request.setStatus(trimmed.isEmpty() ? request.getStatus() : trimmed.toUpper());
                changed = true;
                break;
            }
        }
        if (!changed) return false;
        persist(requests);
        return true;
    }

    core::DynamicArray<model::ReportRequest> ReportService::ensureLoaded() const {
        return repository.loadAll();
    }

    void ReportService::persist(const core::DynamicArray<model::ReportRequest> &requests) const {
        repository.saveAll(requests);
    }

} // namespace pbl2::service
