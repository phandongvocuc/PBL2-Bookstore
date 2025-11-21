#pragma once

#include "core/custom/CustomString.h"

#include "core/custom/DynamicArray.h"

#include "model/Book.h"

namespace pbl2::repository {

class BooksRepository {
public:
    explicit BooksRepository(const custom::CustomString &dataDir);

    [[nodiscard]] custom::DynamicArray<model::Book> loadAll() const;
    void saveAll(const custom::DynamicArray<model::Book> &books) const;

private:
    custom::CustomString dataPath;
};

}  // namespace pbl2::repository
