#include "repository/StaffRepository.h"

#include "datafile/BinaryFileStore.h"
#include "core/PathUtils.h"

using namespace std;

namespace pbl2::repository {

StaffRepository::StaffRepository(const core::CustomString &dataDir)
	: dataPath(core::path::join(dataDir, core::CustomStringLiteral("staffs.bin"))) {}

core::DynamicArray<model::Staff> StaffRepository::loadAll() const {
	return serialization::BinaryFileStore::readStaff(dataPath);
}

void StaffRepository::saveAll(const core::DynamicArray<model::Staff> &staffs) const {
	serialization::BinaryFileStore::writeStaff(staffs, dataPath);
}

}  // namespace pbl2::repository
