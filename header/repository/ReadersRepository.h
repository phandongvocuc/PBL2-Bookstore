#pragma once

#include "core/custom/CustomString.h"

#include "core/custom/DynamicArray.h"

#include "model/Reader.h"

namespace pbl2::repository {

class ReadersRepository {
public:
    explicit ReadersRepository(const custom::CustomString &dataDir);

    [[nodiscard]] custom::DynamicArray<model::Reader> loadAll() const;
    void saveAll(const custom::DynamicArray<model::Reader> &readers) const;

private:
    custom::CustomString dataPath;
};

}  // namespace pbl2::repository
