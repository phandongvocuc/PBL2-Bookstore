#include "service/BookService.h"
#include "BookStatus.h"

namespace pbl2::service {

    BookService::BookService(const core::CustomString &dataDir)
        : BaseService(dataDir) {}

    core::DynamicArray<model::Book> BookService::fetchAll() const {
        return BaseService::fetchAll();
    }

    core::Optional<model::Book> BookService::findById(const core::CustomString &bookId) const {
        return BaseService::findById(bookId);
    }

    bool BookService::addBook(const model::Book &book) const {
        return addItem(book);
    }

    bool BookService::updateBook(const model::Book &book) const {
        return updateItem(book);
    }

    bool BookService::removeBook(const core::CustomString &bookId) const {
        return removeItem(bookId);
    }

    core::DynamicArray<model::Book> BookService::ensureLoaded() const {
        auto books = repository.loadAll();
        bool normalized = false;
        for (auto &book : books) {
            normalized = normalizeAvailability(book) || normalized;
        }
        if (normalized) {
            repository.saveAll(books);
        }
        return books;
    }

    void BookService::persist(const core::DynamicArray<model::Book> &items) const {
        repository.saveAll(items);
    }

    bool BookService::normalizeAvailability(model::Book &book) {
        if (!model::isAvailabilityStatus(book.getStatus())) return false;
        const auto desired = model::availabilityStatusForQuantity(book.getQuantity());
        if (model::canonicalBookStatus(book.getStatus()) == desired) return false;
        book.setStatus(desired);
        return true;
    }

} // namespace pbl2::service