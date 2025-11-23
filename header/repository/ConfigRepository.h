#pragma once

#include "../../core/CustomString.h"

#include "SystemConfig.h"

namespace pbl2::repository {

class ConfigRepository {
public:
    explicit ConfigRepository(const core::CustomString &dataDir);

    [[nodiscard]] model::SystemConfig load() const;
    void save(const model::SystemConfig &config) const;

private:
    core::CustomString dataPath;
};

}  // namespace pbl2::repository
