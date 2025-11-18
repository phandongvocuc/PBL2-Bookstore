#include "repository/StaffRepository.h"

#include "datafile/BinaryFileStore.h"
#include "core/PathUtils.h"

using namespace std;

namespace pbl2::repository {

StaffRepository::StaffRepository(const custom::CustomString &dataDir)
	: dataPath(core::path::join(dataDir, custom::CustomStringLiteral("staffs.bin"))) {}

custom::DynamicArray<model::Staff> StaffRepository::loadAll() const {
	return serialization::BinaryFileStore::readStaff(dataPath);
}

void StaffRepository::saveAll(const custom::DynamicArray<model::Staff> &staffs) const {
	serialization::BinaryFileStore::writeStaff(staffs, dataPath);
}

}  // namespace pbl2::repository
