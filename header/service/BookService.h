#pragma once

#include "core/custom/Optional.h"
#include "core/custom/CustomString.h"
#include "core/custom/DynamicArray.h"

#include "model/Book.h"
#include "repository/BooksRepository.h"
#include "service/BaseService.h"

namespace pbl2::service {

    class BookService : public BaseService<model::Book, repository::BooksRepository> {
    public:
        explicit BookService(const custom::CustomString &dataDir);

        [[nodiscard]] custom::DynamicArray<model::Book> fetchAll() const override;
        [[nodiscard]] custom::Optional<model::Book> findById(const custom::CustomString &bookId) const override;
        [[nodiscard]] bool addBook(const model::Book &book) const;
        [[nodiscard]] bool updateBook(const model::Book &book) const;
        [[nodiscard]] bool removeBook(const custom::CustomString &bookId) const;

    private:
        [[nodiscard]] custom::DynamicArray<model::Book> ensureLoaded() const override;
        void persist(const custom::DynamicArray<model::Book> &items) const override;
        static bool normalizeAvailability(model::Book &book);
    };

} // namespace pbl2::service