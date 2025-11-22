#include "service/ConfigService.h"

namespace pbl2::service {

ConfigService::ConfigService(const core::CustomString &dataDir) : repository(dataDir) {}

model::SystemConfig ConfigService::load() const { return repository.load(); }

bool ConfigService::save(const model::SystemConfig &config) const {
    repository.save(config);
    return true;
}

}  // namespace pbl2::service
