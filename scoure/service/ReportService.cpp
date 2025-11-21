#include "service/ReportService.h"

#include "core/DateTime.h"

using namespace std;

namespace pbl2::service {

ReportService::ReportService(const custom::CustomString &dataDir) : repository(dataDir) {}

custom::DynamicArray<model::ReportRequest> ReportService::fetchAll() const { return ensureLoaded(); }

bool ReportService::submitRequest(model::ReportRequest request) const {
    auto requests = ensureLoaded();

    if (request.getRequestId().trimmed().isEmpty()) {
        const auto now = core::DateTime::nowUtc();
        const custom::CustomString timestamp = now.toCompactTimestamp();
        custom::CustomString id(custom::CustomStringLiteral("RR-"));
        id.append(timestamp);
        request.setRequestId(id);
        request.setCreatedAt(now);
    }

    bool exists = false;
    for (const auto & it : requests) {
        if (it.getRequestId().compare(request.getRequestId(), custom::CaseSensitivity::Insensitive) == 0) {
            exists = true;
            break;
        }
    }
    if (exists) return false;

    if (!request.getCreatedAt().isValid()) {
        request.setCreatedAt(core::DateTime::nowUtc());
    }
    if (request.getStatus().trimmed().isEmpty()) {
        request.setStatus(custom::CustomStringLiteral("PENDING"));
    }

    requests.pushBack(request);
    persist(requests);
    return true;
}

bool ReportService::updateStatus(const custom::CustomString &requestId, const custom::CustomString &status) const {
    const auto requests = ensureLoaded();
    bool changed = false;
    for (auto & request : requests) {
        if (request.getRequestId().compare(requestId, custom::CaseSensitivity::Insensitive) == 0) {
            const custom::CustomString trimmed = status.trimmed();
            request.setStatus(trimmed.isEmpty() ? request.getStatus() : trimmed.toUpper());
            changed = true;
            break;
        }
    }
    if (!changed) return false;
    persist(requests);
    return true;
}

custom::DynamicArray<model::ReportRequest> ReportService::ensureLoaded() const {
    return repository.loadAll();
}

void ReportService::persist(const custom::DynamicArray<model::ReportRequest> &requests) const {
    repository.saveAll(requests);
}

}  // namespace pbl2::service
