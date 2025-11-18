#include "repository/ReportRequestRepository.h"

#include "datafile/BinaryFileStore.h"
#include "core/PathUtils.h"

using namespace std;

namespace pbl2::repository {

ReportRequestRepository::ReportRequestRepository(const custom::CustomString &dataDir)
	: dataPath(core::path::join(dataDir, custom::CustomStringLiteral("report_requests.bin"))) {}

custom::DynamicArray<model::ReportRequest> ReportRequestRepository::loadAll() const {
	return serialization::BinaryFileStore::readReports(dataPath);
}

void ReportRequestRepository::saveAll(const custom::DynamicArray<model::ReportRequest> &requests) const {
	serialization::BinaryFileStore::writeReports(requests, dataPath);
}

}  // namespace pbl2::repository
