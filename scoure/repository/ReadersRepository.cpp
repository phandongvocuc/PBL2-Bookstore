#include "repository/ReadersRepository.h"

#include "datafile/BinaryFileStore.h"
#include "core/PathUtils.h"

using namespace std;

namespace pbl2::repository {

ReadersRepository::ReadersRepository(const core::CustomString &dataDir)
	: dataPath(core::path::join(dataDir, core::CustomStringLiteral("readers.bin"))) {}

core::DynamicArray<model::Reader> ReadersRepository::loadAll() const {
	return serialization::BinaryFileStore::readReaders(dataPath);
}

void ReadersRepository::saveAll(const core::DynamicArray<model::Reader> &readers) const {
	serialization::BinaryFileStore::writeReaders(readers, dataPath);
}

}  // namespace pbl2::repository
