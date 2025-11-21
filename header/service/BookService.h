#pragma once

#include "core/custom/Optional.h"

#include "core/custom/CustomString.h"

#include "core/custom/DynamicArray.h"

#include "model/Book.h"
#include "repository/BooksRepository.h"

namespace pbl2::service {

class BookService {
public:
    explicit BookService(const custom::CustomString &dataDir);

    [[nodiscard]] custom::DynamicArray<model::Book> fetchAll() const;
    [[nodiscard]] custom::Optional<model::Book> findById(const custom::CustomString &bookId) const;
    [[nodiscard]] bool addBook(const model::Book &book) const;
    [[nodiscard]] bool updateBook(const model::Book &book) const;
    [[nodiscard]] bool removeBook(const custom::CustomString &bookId) const;

private:
    repository::BooksRepository repository;

    [[nodiscard]] custom::DynamicArray<model::Book> ensureLoaded() const;
    void persist(const custom::DynamicArray<model::Book> &books) const;
    static bool normalizeAvailability(model::Book &book);
};

}  // namespace service
