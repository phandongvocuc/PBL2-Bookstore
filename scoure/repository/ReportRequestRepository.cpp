#include "repository/ReportRequestRepository.h"

#include "datafile/BinaryFileStore.h"
#include "core/PathUtils.h"

using namespace std;

namespace pbl2::repository {

ReportRequestRepository::ReportRequestRepository(const core::CustomString &dataDir)
	: dataPath(core::path::join(dataDir, core::CustomStringLiteral("report_requests.bin"))) {}

core::DynamicArray<model::ReportRequest> ReportRequestRepository::loadAll() const {
	return serialization::BinaryFileStore::readReports(dataPath);
}

void ReportRequestRepository::saveAll(const core::DynamicArray<model::ReportRequest> &requests) const {
	serialization::BinaryFileStore::writeReports(requests, dataPath);
}

}  // namespace pbl2::repository
