#include "repository/ConfigRepository.h"

#include "datafile/BinaryFileStore.h"
#include "core/PathUtils.h"

using namespace std;

namespace pbl2::repository {

ConfigRepository::ConfigRepository(const core::CustomString &dataDir)
	: dataPath(core::path::join(dataDir, core::CustomStringLiteral("config.bin"))) {}

model::SystemConfig ConfigRepository::load() const {
	return serialization::BinaryFileStore::readConfig(dataPath);
}

void ConfigRepository::save(const model::SystemConfig &config) const {
	serialization::BinaryFileStore::writeConfig(config, dataPath);
}

}  // namespace pbl2::repository
