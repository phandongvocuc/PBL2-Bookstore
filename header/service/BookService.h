#pragma once

#include "../../core/Optional.h"
#include "../../core/CustomString.h"
#include "../../core/DynamicArray.h"

#include "model/Book.h"
#include "repository/BooksRepository.h"
#include "service/BaseService.h"

namespace pbl2::service {

    class BookService : public BaseService<model::Book, repository::BooksRepository> {
    public:
        explicit BookService(const core::CustomString &dataDir);

        [[nodiscard]] core::DynamicArray<model::Book> fetchAll() const override;
        [[nodiscard]] core::Optional<model::Book> findById(const core::CustomString &bookId) const override;
        [[nodiscard]] bool addBook(const model::Book &book) const;
        [[nodiscard]] bool updateBook(const model::Book &book) const;
        [[nodiscard]] bool removeBook(const core::CustomString &bookId) const;

    private:
        [[nodiscard]] core::DynamicArray<model::Book> ensureLoaded() const override;
        void persist(const core::DynamicArray<model::Book> &items) const override;
        static bool normalizeAvailability(model::Book &book);
    };

} // namespace pbl2::service
