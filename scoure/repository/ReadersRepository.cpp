#include "repository/ReadersRepository.h"

#include "datafile/BinaryFileStore.h"
#include "core/PathUtils.h"

using namespace std;

namespace pbl2::repository {

ReadersRepository::ReadersRepository(const custom::CustomString &dataDir)
	: dataPath(core::path::join(dataDir, custom::CustomStringLiteral("readers.bin"))) {}

custom::DynamicArray<model::Reader> ReadersRepository::loadAll() const {
	return serialization::BinaryFileStore::readReaders(dataPath);
}

void ReadersRepository::saveAll(const custom::DynamicArray<model::Reader> &readers) const {
	serialization::BinaryFileStore::writeReaders(readers, dataPath);
}

}  // namespace pbl2::repository
