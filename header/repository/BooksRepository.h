#pragma once

#include "../../core/CustomString.h"
#include "../../core/DynamicArray.h"

#include "model/Book.h"

namespace pbl2::repository {

class BooksRepository {
public:
    explicit BooksRepository(const core::CustomString &dataDir);

    [[nodiscard]] core::DynamicArray<model::Book> loadAll() const;
    void saveAll(const core::DynamicArray<model::Book> &books) const;

private:
    core::CustomString dataPath;
};

}  // namespace pbl2::repository
