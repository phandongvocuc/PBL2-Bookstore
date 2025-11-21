#pragma once

#include "core/custom/CustomString.h"

#include "SystemConfig.h"
#include "repository/ConfigRepository.h"

namespace pbl2::service {

class ConfigService {
public:
    explicit ConfigService(const custom::CustomString &dataDir);

    [[nodiscard]] model::SystemConfig load() const;
    [[nodiscard]] bool save(const model::SystemConfig &config) const;

private:
    repository::ConfigRepository repository;
};

}  // namespace pbl2::service
