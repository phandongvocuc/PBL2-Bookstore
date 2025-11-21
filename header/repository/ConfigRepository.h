#pragma once

#include "core/custom/CustomString.h"

#include "SystemConfig.h"

namespace pbl2::repository {

class ConfigRepository {
public:
    explicit ConfigRepository(const custom::CustomString &dataDir);

    [[nodiscard]] model::SystemConfig load() const;
    void save(const model::SystemConfig &config) const;

private:
    custom::CustomString dataPath;
};

}  // namespace pbl2::repository
