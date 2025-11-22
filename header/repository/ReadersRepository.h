#pragma once

#include "../../core/CustomString.h"
#include "../../core/DynamicArray.h"

#include "model/Reader.h"

namespace pbl2::repository {

class ReadersRepository {
public:
    explicit ReadersRepository(const core::CustomString &dataDir);

    [[nodiscard]] core::DynamicArray<model::Reader> loadAll() const;
    void saveAll(const core::DynamicArray<model::Reader> &readers) const;

private:
    core::CustomString dataPath;
};

}  // namespace pbl2::repository
